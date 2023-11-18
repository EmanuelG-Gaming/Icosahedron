#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/math/geom/Quaternion.h>

#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/gl/Framebuffer.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/3d/Mesh3D.h>
#include <Icosahedron/scene/3d/Camera3D.h>
#include <Icosahedron/scene/3d/controllers/FreeRoamCameraController3D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>


std::string screenVertex = IC_ADD_GLSL_DEFINITION(
    layout (location = 0) in vec2 position;
    layout (location = 1) in vec2 tCoords;

    out vec2 vPosition;
    out vec2 vTCoords;

    void main() {
        vPosition = position;
        vTCoords = vec2(tCoords.x, 1.0 - tCoords.y);
        
        gl_Position = vec4(vPosition, 0.0, 1.0);
    }
);

std::string screenFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec2 vPosition;
    in vec2 vTCoords;

    const float gamma = 2.2;
    uniform float exposure = 0.05;

    uniform sampler2D screenTexture;

    out vec4 outColor;

    void main() {
        vec3 hdrColor = texture(screenTexture, vTCoords).rgb;
        
        // Tone mapping
        //vec3 mapped = vec3(1.0) - exp(hdrColor * -exposure);

        // Aces
        hdrColor *= exposure;
        float a = 2.51;
        float b = 0.03;
        float c = 2.43;
        float d = 0.59;
        float e = 0.14;

        vec3 mapped = clamp((hdrColor * (a * hdrColor + b)) / (hdrColor * (c * hdrColor + d) + e), 0.0, 1.0);

        // Gamma correction
        mapped = pow(mapped, vec3(1.0 / gamma));

        outColor = vec4(mapped, 1.0);
    }
);

std::string fragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;
    in vec2 vTCoords;
    in vec3 vNormal;
    struct PointLight {
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float constant;
        float linear;
        float quadratic;
    };
    PointLight l = PointLight(
        vec3(-2.0, 3.0, -3.0), 
        vec3(2.0, 2.0, 2.0), 
        vec3(9.0, 8.0, 6.5), 
        vec3(8.0, 7.0, 5.5), 
        
        //1.0, 0.09, 0.032
        0.01, 0.02, 0.1
    );

    uniform sampler2D sampleTexture;
    uniform vec3 viewPosition;
    out vec4 outColor;

    vec4 compute_lighting(PointLight light) {
        float distance = length(light.position - vPosition);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        vec3 normal = normalize(vNormal);
        vec3 lightDirection = normalize(light.position - vPosition);
        vec3 viewDirection = normalize(viewPosition - vPosition);
        float dotProduct = clamp(dot(lightDirection, normal), 0.0, 1.0);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4 * attenuation;
        vec4 ambientColor = vec4(light.ambient, 1.0) * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0) * attenuation;
        vec4 diffuseColor = texture(sampleTexture, vTCoords);
        if (diffuseColor.a <= 0.1) diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
        else diffuseColor *= vec4(light.diffuse, 1.0) * diffuseIntensity;

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        vec4 specularColor = vec4(light.specular, 1.0) * specularIntensity;
        vec4 result = ambientColor + diffuseColor + specularColor;
        return result;
    }
    
    void main() {
        vec4 color = compute_lighting(l);
        
        outColor = color;
    }
);


/** High dynamic range (HDR) allows the shader to use larger color values.
 *  Use the mouse wheel to change the exposure of the camera.
 */
class HDR : public ic::Application {
    ic::Shader shader, screenShader;
    ic::Framebuffer framebuffer;
    
    ic::Camera3D camera;
    ic::Mesh2D screenQuad;
    ic::Mesh3D mesh, floorMesh;

    ic::Texture floorTexture, whiteTexture;
    ic::FreeRoamCameraController3D controller;

    float time, exposure;

    public:
        bool init() override {
            displayName = "HDR example";
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            states.enable_face_culling(ic::FRONT, ic::CCW);
            
            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex3D, fragment);
            screenShader = ic::ShaderLoader::get().load(screenVertex, screenFragment);
            
            ic::TextureParameters params;
            params.usesMipmapping = true;

            // Note that we use sRGB textures
            floorTexture = ic::TextureLoader::get().load_png("resources/textures/wood.png", params, true);
            whiteTexture = ic::TextureLoader::get().load_png("resources/textures/white.png", params, true);
            
            framebuffer = ic::Framebuffer(ic::TEXTURE_ATTACH_COLOR_0, ic::TEXTURE_RGBA_16F, ic::TEXTURE_RGBA, IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);

            mesh = ic::GeometryGenerator::get().generate_cube_mesh(0.5f);
            floorMesh = ic::GeometryGenerator::get().generate_parallelipiped_mesh(25.0f, 0.1f, 25.0f, 50.0f, 0.2f, 50.0f);
            
            screenQuad = ic::Mesh2D();
            screenQuad.add_attribute(0, 2, ic::GeometryGenerator::get().generate_rectangle(1.0f, 1.0f));
            screenQuad.add_attribute(1, 2, ic::GeometryGenerator::get().generate_UV_rectangle());
            screenQuad.set_index_buffer({ 0, 1, 2, 0, 2, 3 });

            camera = ic::Camera3D();
            camera.position = { -3.0f, 1.5f, 0.0f };
            controller = ic::FreeRoamCameraController3D(&camera);
            controller.flying = true;


            ic::MouseController *mouse = new ic::MouseController();
            mouse->add_mouse_scroll_up_action([this]() { 
                float p = ic::InputHandler::get().find_mouse("mouse")->get_wheel_direction() * 0.05f;
                exposure = std::max(0.01f, std::min(exposure + p, 5.0f));
            });
            mouse->add_mouse_scroll_down_action([this]() { 
                float p = ic::InputHandler::get().find_mouse("mouse")->get_wheel_direction() * 0.05f;
                exposure = std::max(0.01f, std::min(exposure + p, 5.0f));
            });
            ic::InputHandler::get().add_input(mouse, "mouse");

            time = 0.0f;
            exposure = 0.05;

            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        void window_size_changed(int w, int h) override {
            camera.resize(w, h);
            framebuffer.resize(w, h);
        }

        bool update(float dt) override {
            time += dt;

            controller.act(dt);
            camera.update();
            
            // First pass - scene drawing
            framebuffer.use();
            clear_color(ic::Colors::blue);
            
            shader.use();
            shader.set_uniform_vec3f("viewPosition", camera.position);
            camera.upload_to_shader(shader);
            

            ic::Quaternion quat = ic::Quaternion().from_euler(0.0f, time, 0.0f);
            ic::Mat4x4 rotation = quat.to_rotation_matrix();
            ic::Mat4x4 translation = ic::Mat4x4().set_translation<3>({0.0f, 0.6f, 0.0f});
            mesh.set_transformation(translation * rotation);
            mesh.set_normal_transformation(rotation);
            

            whiteTexture.use();
            mesh.draw(shader);

            floorTexture.use();

            floorMesh.draw(shader);

            framebuffer.unuse();


            // Second pass - drawing via framebuffer
            clear_color(ic::Colors::cyan);
            
            screenShader.use();
            screenShader.set_uniform_float("exposure", exposure);

            framebuffer.use_texture();
            screenQuad.draw(screenShader);

            return true; 
        }

        void dispose() override {
            shader.clear();
            mesh.dispose();
            floorMesh.dispose();
            floorTexture.dispose();
            whiteTexture.dispose();
            framebuffer.dispose();
        }
};


int main(int argc, char *argv[]) {
    HDR application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}