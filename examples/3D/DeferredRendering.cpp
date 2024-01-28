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


// GShader

std::string gShaderFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    layout (location = 0) out vec3 outPosition;
    layout (location = 1) out vec3 outNormal;
    layout (location = 2) out vec3 outAlbedo;

    in vec3 vPosition;
    in vec3 vNormal;
    in vec2 vTCoords;

    uniform sampler2D sampleTexture;
    
    void main() {
        outPosition = vPosition;
        outNormal = normalize(vNormal);
        outAlbedo = texture(sampleTexture, vTCoords).rgb;
    }
);



// Screen-space shader

std::string screenLaterVertex = IC_ADD_GLSL_DEFINITION(
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

std::string laterCompileFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;

    
    uniform sampler2D gPosition;
    uniform sampler2D gNormal;
    uniform sampler2D gAlbedo;

    in vec2 vTCoords;

    const int MAX_POINT_LIGHTS = 100;

    struct PointLight {
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float constant;
        float linear;
        float quadratic;
    };
    uniform PointLight pointLights[MAX_POINT_LIGHTS];


    uniform vec3 viewPosition;

    out vec4 outColor;
    
    vec3 compute_lighting(PointLight light, in vec3 pos, in vec3 normal, in vec3 albedo) {
        vec3 difference = light.position - pos;
        float distance = length(difference);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        vec3 lightDirection = difference / distance;
        vec3 viewDirection = normalize(viewPosition - pos);
        float dotProduct = clamp(dot(lightDirection, normal), 0.0, 1.0);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4 * attenuation;
        vec3 ambientColor = light.ambient * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0) * attenuation;
        vec3 diffuseColor = albedo * light.diffuse * diffuseIntensity;

        vec3 result = ambientColor + diffuseColor;
        return result;
    }
    
    void main() {
        vec3 pos = texture(gPosition, vTCoords).rgb;
        vec3 normal = texture(gNormal, vTCoords).rgb;
        vec3 albedo = texture(gAlbedo, vTCoords).rgb;

        vec3 lighting = vec3(0.0);
        for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
            lighting += compute_lighting(pointLights[i], pos, normal, albedo);
        }

        outColor = vec4(lighting, 1.0);
    }
);

// In deferred rendering, the light calculations are done later.
// First, objects are being rendered on a framebuffer (a.k.a. the g-buffer) that fills in the position and normal of screen pixels,
// then a shader does the light calculations using those pixels. 
// This way, lighting can be faster, as the position and normals are encoded in the g buffer once and the shading can be iteratively done for each pixel.
class DeferredRendering : public ic::Application {
    ic::Shader gShader, laterShader;
    ic::Framebuffer gBuffer;

    ic::Camera3D camera;
    ic::Mesh2D screenQuad;
    ic::Mesh3D mesh, floorMesh;

    ic::Texture floorTexture, meshTexture;
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
            laterShader = ic::ShaderLoader::get().load(screenLaterVertex, laterCompileFragment);
            
            // Shader configuration
            laterShader.use();
            laterShader.set_uniform_int("gPosition", 0);
            laterShader.set_uniform_int("gNormal", 1);
            laterShader.set_uniform_int("gAlbedo", 2);

            for (int i = 0; i < 100; i++) {
                std::string strIndex = std::to_string(i);

                laterShader.set_uniform_vec3f("pointLights[" + strIndex + "].position", { rand() % 40 - 20.0f, 2.0f, rand() % 40 - 20.0f });
                laterShader.set_uniform_vec3f("pointLights[" + strIndex + "].ambient", { 0.01f, 0.01f, 0.01f });
                laterShader.set_uniform_vec3f("pointLights[" + strIndex + "].diffuse", { rand() % 255 / 255.0f + 0.1f, rand() % 255 / 255.0f + 0.1f, rand() % 255 / 255.0f + 0.1f });
                laterShader.set_uniform_vec3f("pointLights[" + strIndex + "].specular", { 1.0f, 1.0f, 1.0f });

                laterShader.set_uniform_float("pointLights[" + strIndex + "].constant", 1.0f);
                laterShader.set_uniform_float("pointLights[" + strIndex + "].linear", 0.8f);
                laterShader.set_uniform_float("pointLights[" + strIndex + "].quadratic", 0.32f);
            }

            // We want three-component position and normals, so RGB colours work for this case
            gBuffer = ic::Framebuffer(ic::TEXTURE_ATTACH_COLOR_0, ic::TEXTURE_RGB_16F, ic::TEXTURE_RGB, IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            gBuffer.add_render_target(ic::TEXTURE_ATTACH_COLOR_1, ic::TEXTURE_RGB_16F, ic::TEXTURE_RGB);

            // Albedo textures
            gBuffer.add_render_target(ic::TEXTURE_ATTACH_COLOR_2, ic::TEXTURE_RGBA_16F, ic::TEXTURE_RGBA);


            ic::TextureParameters params;
            params.usesMipmapping = true;

            floorTexture = ic::TextureLoader::get().load_png("resources/textures/wood.png", params);
            meshTexture = ic::TextureLoader::get().load_png("resources/textures/sand.png", params);
            
            
            mesh = ic::GeometryGenerator::get().generate_cube_mesh(0.5f);
            mesh = ic::GeometryGenerator::get().generate_UV_sphere_mesh(0.5f, 18, 18);

            floorMesh = ic::GeometryGenerator::get().generate_parallelipiped_mesh(25.0f, 0.1f, 25.0f, 50.0f, 0.2f, 50.0f);
            
            screenQuad = ic::Mesh2D();
            screenQuad.add_attribute(0, 2, ic::GeometryGenerator::get().generate_rectangle(1.0f, 1.0f));
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
            gBuffer.use_texture(2, 2);
            screenQuad.draw(laterShader);


            return true; 
        }

        void dispose() override {
            
        }

        void render_scene(ic::Shader &shader) {
            floorMesh.set_transformation(ic::Mat4x4().set_translation<3>({0.0f, 0.0f, 0.0f}));

            meshTexture.use();
            for (int j = 0; j < 20; j++) {
                for (int i = 0; i < 20; i++) {
                    float x = (i - 10.0f) * 2.0f;
                    float z = (j - 10.0f) * 2.0f;

                    mesh.set_transformation(ic::Mat4x4().set_translation<3>({x, 0.6f, z}));
                    mesh.draw(shader);
                }
            }

            floorTexture.use();
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