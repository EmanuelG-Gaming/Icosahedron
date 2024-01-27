#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/math/geom/Quaternion.h>

#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Framebuffer.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/3d/Mesh3D.h>
#include <Icosahedron/scene/3d/Camera3D.h>
#include <Icosahedron/scene/3d/controllers/FreeRoamCameraController3D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>
#include <IcosahedronDebug/ConsoleOutput.h>


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

std::string gShaderFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    layout (location = 0) out vec3 outPosition;
    layout (location = 1) out vec3 outNormal;

    in vec3 vPosition;
    in vec3 vNormal;
    
    
    void main() {
        outPosition = vPosition;
        outNormal = normalize(vNormal);
    }
);

std::string laterCompileFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;

    
    uniform sampler2D gPosition;
    uniform sampler2D gNormal;

    in vec2 vTCoords;

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
        vec3(-2.0, 0.0, -3.0), 

        vec3(0.2, 0.2, 0.2), 
        vec3(0.0, 0.0, 0.95), 
        vec3(0.8, 0.7, 0.55), 
        
        1.0, 0.09, 0.032
    );


    uniform vec3 viewPosition;

    out vec4 outColor;
    
    vec3 compute_lighting(PointLight light, in vec3 pos, in vec3 normal) {
        float distance = length(light.position - pos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        vec3 lightDirection = normalize(light.position - pos);
        vec3 viewDirection = normalize(viewPosition - pos);
        float dotProduct = clamp(dot(lightDirection, normal), 0.0, 1.0);
        
        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4 * attenuation;
        vec3 ambientColor = light.ambient * ambientIntensity;
        
        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0) * attenuation;
        vec3 diffuseColor = light.diffuse * diffuseIntensity;
        
        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(normal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        vec3 specularColor = light.specular * ambientIntensity;
        vec3 result = ambientColor + diffuseColor + specularColor;
        return result;
        //return vec3(1.0, 1.0, 0.0);
    }
    
    void main() {
        vec3 pos = texture(gPosition, vTCoords).rgb;
        vec3 normal = texture(gNormal, vTCoords).rgb;

        vec3 lighting = compute_lighting(l, pos, normal);
        //outColor = vec4(lighting, 1.0);
        outColor = vec4(normal + pos, 1.0);
    }
);

// In deferred rendering, the light calculations are done later.
// First, objects are being rendered on a framebuffer (a.k.a. the g-buffer) that fills in the position and normal of screen pixels,
// then a shader does the light calculations using those pixels. 
class DeferredRendering : public ic::Application {
    ic::Shader gShader, laterShader;
    ic::Framebuffer gBuffer;

    ic::Camera3D camera;
    ic::Mesh2D screenQuad;
    ic::Mesh3D mesh, floorMesh;

    ic::Texture floorTexture, whiteTexture;
    ic::FreeRoamCameraController3D controller;

    float time = 0.0f;

    public:
        bool init() override {
            displayName = "Deferred shading";
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            states.enable_face_culling(ic::FRONT, ic::CCW);
            

            gShader = ic::ShaderLoader::get().load(shaders.meshShaderVertex3D, gShaderFragment);
            laterShader = ic::ShaderLoader::get().load(screenVertex, laterCompileFragment);
            
            // Shader configuration
            laterShader.use();
            laterShader.set_uniform_int("gPosition", 0);
            laterShader.set_uniform_int("gNormal", 1);


            gBuffer = ic::Framebuffer(ic::TEXTURE_ATTACH_COLOR_0, ic::TEXTURE_RGB, ic::TEXTURE_RGB, IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            gBuffer.add_render_target(ic::TEXTURE_ATTACH_COLOR_1, ic::TEXTURE_RGB, ic::TEXTURE_RGB);


            ic::TextureParameters params;
            params.usesMipmapping = true;

            floorTexture = ic::TextureLoader::get().load_png("resources/textures/wood.png", params, true);
            whiteTexture = ic::TextureLoader::get().load_png("resources/textures/white.png", params, true);
            
            
            mesh = ic::GeometryGenerator::get().generate_cube_mesh(0.5f);
            mesh = ic::GeometryGenerator::get().generate_UV_sphere_mesh(2.0f, 18, 18);

            floorMesh = ic::GeometryGenerator::get().generate_parallelipiped_mesh(25.0f, 0.1f, 25.0f, 50.0f, 0.2f, 50.0f);
            
            screenQuad = ic::Mesh2D();
            screenQuad.add_attribute(0, 2, ic::GeometryGenerator::get().generate_rectangle(0.95f, 1.0f));
            screenQuad.add_attribute(1, 2, ic::GeometryGenerator::get().generate_UV_rectangle(1.0f, 1.0f));
            screenQuad.set_index_buffer({ 0, 1, 2, 0, 2, 3 });

            
            camera = ic::Camera3D();
            camera.position = { -3.0f, 1.5f, 0.0f };
            controller = ic::FreeRoamCameraController3D(&camera);
            controller.flying = true;


            return true;
        }

        void window_size_changed(int w, int h) override {
            camera.resize(w, h);
            gBuffer.resize(w, h);
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            time += dt;

            controller.act(dt);
            camera.update();
            
            clear_color(ic::Colors::blue);

            gBuffer.use();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            gShader.use();
            gShader.set_uniform_vec3f("viewPosition", camera.position);
            camera.upload_to_shader(gShader);
            render_scene(gShader);

            gBuffer.unuse();


            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            laterShader.use();

            // Use both rendering targets from the framebuffer
            gBuffer.use_texture(0, 0);
            gBuffer.use_texture(1, 1);
            screenQuad.draw(laterShader);


            return true; 
        }

        void dispose() override {
            
        }

        void render_scene(ic::Shader &shader) {
            mesh.set_transformation(ic::Mat4x4().set_translation<3>({-1.0f + sin(time * 0.5f) * 15.0f, 1.0f + sin(time) * 5.0f, -0.5f + sin(time * 2.0f) * 8.0f}));
            floorMesh.set_transformation(ic::Mat4x4().set_translation<3>({0.0f, 0.0f, 0.0f}));

            mesh.draw(shader);
            floorMesh.draw(shader);
        }
};

int main() {
    DeferredRendering application;
    //ic::Debug::ConsoleOutput::get().write_file("yet.txt", stdout);

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}