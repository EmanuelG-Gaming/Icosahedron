#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/3d/Camera3D.h>
#include <Icosahedron/scene/3d/Mesh3D.h>
#include <Icosahedron/scene/3d/Skybox.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/scene/3d/controllers/FreeRoamCameraController3D.h>
#include <IcosahedronDebug/ConsoleOutput.h>
#include <Icosahedron/assets/loaders/OBJLoader.h>


std::string fragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;
    in vec2 vTCoords;
    in vec3 vNormal;
    struct DirectionalLight {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

    DirectionalLight l = DirectionalLight(
        vec3(5.0, -8.0, 5.0), 
        vec3(0.7, 0.7, 0.9), 
        vec3(1.0, 1.0, 0.95), 
        vec3(0.5, 0.5, 0.5)
    );
    
    uniform vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
    uniform samplerCube skyboxSampler;
    uniform bool hasDiffuseShading = true;

    uniform float reflectivity = 0.2f;
    uniform float refractionRatio = 1.0 / 1.518;

    uniform vec3 viewPosition;
    out vec4 outColor;

    vec4 compute_lighting(DirectionalLight light) {
        vec3 normal = normalize(vNormal);
        vec3 lightDirection = normalize(-light.direction);
        vec3 viewDirection = normalize(viewPosition - vPosition);
        float dotProduct = dot(lightDirection, normal);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.5;
        vec4 ambientColor = vec4(light.ambient, 1.0) * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0);
        vec3 diffusePart = hasDiffuseShading ? light.diffuse * diffuseIntensity : vec3(1.0);
        vec4 diffuse = vec4((texture(skyboxSampler, reflect(-viewDirection, normal)) * reflectivity + texture(skyboxSampler, refract(-viewDirection, normal, refractionRatio)) * (1.0 - reflectivity)).rgb * diffuseColor * diffusePart, 1.0);

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0);
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0);
        
        vec4 specularColor = vec4(light.specular, 1.0) * specularIntensity;
        vec4 result = ambientColor + diffuse + specularColor;
        return result;
    }
    
    void main() {
        vec4 color = compute_lighting(l);
        outColor = color;
    }
);

const std::string skyboxPath = "resources/textures/skybox/";

/** Showcases skyboxes and environmental mapping (using the skybox for light interations such as reflections, refractions, etc). */
class Skyboxes : public ic::Application {
    ic::Color meshColor, mesh2Color, floorColor;
    ic::Shader shader, skyShader;

    ic::Camera3D camera;
    ic::FreeRoamCameraController3D controller;

    ic::Mesh3D mesh, mesh2, floorMesh;

    ic::Skybox skybox;
    float time = 0.0f;

    public:
        bool init() override {
            displayName = "Skybox showcasing";
            hideCursor = true;
            
            return true;
        }
        
        bool load() override {
            ic::GLStateHandler::enable_depth_testing(ic::LESS);
            ic::GLStateHandler::enable_face_culling(ic::FRONT, ic::CCW);

            
            shader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex3D, fragment);
            skyShader = ic::ShaderLoader::load(ic::Shaders::skyboxVertex, ic::Shaders::skyboxFragment);


            mesh = ic::GeometryGenerator::generate_UV_sphere_mesh(0.5f, 100, 100);
            mesh2 = ic::OBJLoader::load("resources/models/monkey.obj");
            
            skybox = ic::Skybox(std::vector<std::string>({ 
                skyboxPath + "right.bmp",
                skyboxPath + "left.bmp",
                skyboxPath + "top.bmp",
                skyboxPath + "bottom.bmp",
                skyboxPath + "front.bmp",
                skyboxPath + "back.bmp",
            }), true);

            floorMesh = ic::GeometryGenerator::generate_parallelipiped_mesh(20.0f, 0.1f, 20.0f);


            camera = ic::Camera3D();
            camera.position = { -8.0f, 4.0f, 4.0f };
            camera.lookingAt = { 0.0f, 0.5f, 0.0f };

            controller = ic::FreeRoamCameraController3D(&camera);
            controller.flying = true;

            meshColor = ic::Colors::white;
            floorColor = ic::Color().hexadecimal_to_RGB("268B07");
            mesh2Color = ic::Colors::yellow;

           
            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }


        bool update(float dt) override {
            time += dt;

            controller.act(dt);
            camera.update();
            
            clear_color(ic::Colors::blue);

            // Skybox rendering
            skyShader.use();
            camera.upload_to_shader(skyShader);
            skybox.use_texture();
            skybox.draw(skyShader);


            // Scene rendering
            shader.use();
            shader.set_uniform_vec3f("viewPosition", camera.position);
            camera.upload_to_shader(shader);

            skybox.use_texture();
            
            mesh.set_transformation(ic::Mat4x4().set_translation<3>({0.0f, 0.6f, 0.0f}));
            shader.set_uniform_color("diffuseColor", meshColor);
            mesh.draw(shader);

            mesh2.set_transformation(ic::Mat4x4().set_translation<3>({10.0f, 2.0f, -5.0f}));
            shader.set_uniform_color("diffuseColor", mesh2Color);
            mesh2.draw(shader);

            shader.set_uniform_color("diffuseColor", floorColor);
            shader.set_uniform_float("reflectivity", 0.0f);
            shader.set_uniform_float("refractiveRatio", 1 / 1000.0f);
            floorMesh.draw(shader);

            return true; 
        }

        void dispose() override {
            shader.clear();
            skyShader.clear();

            mesh.dispose();
            mesh2.dispose();
            floorMesh.dispose();
        }
};


int main() {
    Skyboxes application;
    //ic::Debug::ConsoleOutput::get().write_file("yet.txt", stdout);

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}