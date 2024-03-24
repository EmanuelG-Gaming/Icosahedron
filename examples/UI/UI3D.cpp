#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/3d/Camera3D.h>
#include <Icosahedron/scene/3d/Mesh3D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>

#include <Icosahedron/scene/3d/controllers/FreeRoamCameraController3D.h>
#include <IcosahedronDebug/ConsoleOutput.h>


#include <UI/Core.h>
#include <UI/Label.h>
#include <UI/Button.h>
#include <UI/style/TextureDrawable.h>



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
        vec3(2.0, -2.0, -2.0), 
        vec3(0.2, 0.2, 0.2), 
        vec3(0.8, 0.8, 0.8), 
        vec3(0.5, 0.5, 0.5)
    );

    uniform sampler2D textureSample;

    uniform vec3 viewPosition;
    out vec4 outColor;

    vec4 compute_lighting(DirectionalLight light) {
        vec3 normal = normalize(vNormal);
        vec3 lightDirection = normalize(-light.direction);
        vec3 viewDirection = normalize(viewPosition - vPosition);
        float dotProduct = dot(lightDirection, normal);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4;
        vec4 ambientColor = vec4(light.ambient * ambientIntensity, 0.0);

        // Diffuse reflection
        vec4 albedo = texture(textureSample, vTCoords);
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0);
        vec4 diffuseColor = albedo * vec4(light.diffuse * diffuseIntensity, 1.0);

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0);
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0);
        
        vec4 specularColor = vec4(light.specular, albedo.a) * specularIntensity;
        vec4 result = ambientColor + diffuseColor + specularColor;
        return result;
    }
    
    void main() {
        vec4 color = compute_lighting(l);
        outColor = color;
    }
);

enum GameStates {
    MENU,
    PLAYING,
    SETTINGS,
};

class UI3D : public ic::Application {
    ic::Shader shader;
    ic::Texture windowTexture, floorTexture;

    ic::Camera3D camera;
    ic::FreeRoamCameraController3D controller;

    std::vector<ic::Vec3f> windows;

    ic::Mesh3D mesh, floorMesh;

    ic::UI::TextureDrawable *buttonDrawable, *tinted;

    GameStates state;

    public:
        bool init() override {
            displayName = "3D transparency with menu";
            //hideCursor = true;

            return true;
        }
        
        bool load() override {
            ic::GLStateHandler::enable_depth_testing(ic::LESS);
            ic::GLStateHandler::enable_blending(ic::SRC_ALPHA, ic::DEST_ONE_MINUS_SRC_ALPHA);

            state = MENU;

            ic::TextureParameters params;
            params.usesMipmapping = true;

            shader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex3D, fragment);
            windowTexture = ic::TextureLoader::load_png("resources/textures/window.png", params);
            floorTexture = ic::TextureLoader::load_png("resources/textures/wood.png", params);

            std::vector<float> positions = {
                0.0f, -0.5f, -0.5f,
                0.0f, 0.5f, -0.5f, 
                0.0f, 0.5f, 0.5f,
                0.0f, -0.5f, 0.5f, 
            };

            std::vector<float> textureCoords = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f,
            };

            std::vector<float> normals = {
                1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
            };

            mesh = ic::Mesh3D();
            mesh.add_attribute(0, 3, positions);
            mesh.add_attribute(2, 2, textureCoords);
            mesh.add_attribute(3, 3, normals);
            mesh.set_index_buffer({ 0, 1, 2, 0, 2, 3 });

            for (int i = 0; i < 1000; i++) {
                windows.push_back({ rand() % 5000 / 5000.0f * 40.0f - 20.0f, 0.7f, rand() % 5000 / 5000.0f * 40.0f - 20.0f });
            }


            floorMesh = ic::GeometryGenerator::generate_parallelipiped_mesh(20.0f, 0.2f, 20.0f, 40.0f, 0.4f, 40.0f);

            camera = ic::Camera3D();
            camera.position = { 2.0f, 2.0f, 2.0f };
            camera.lookingAt = { 3.0f, 2.0f, 2.0f };

            controller = ic::FreeRoamCameraController3D(&camera);
            controller.flying = true;


            static auto &ui = ic::UI::Core::get();
            ui.load();
            ui.atlas->add_entry("wood", "resources/textures/wood.png");
            ui.atlas->add_entry("ball", "resources/textures/ball.png");
            ui.atlas->add_entry("white", "resources/textures/white.png");

            srand(time(NULL));

            buttonDrawable = new ic::UI::TextureDrawable("wood");
            tinted = new ic::UI::TextureDrawable("white");
            tinted->tint = ic::Colors::black;
            tinted->tint.a = 150.0f;
            

            {
                ic::UI::Table *tab = new ic::UI::Table(tinted);

                ic::UI::Button *button1 = tab->text_button("Play");
                button1->clickListener = [=]() {
                    this->state = PLAYING;
                    ui.mainTable->remove(tab);
                };
                button1->set_position(0.0f, 0.2f)->set_background(buttonDrawable);


                ic::UI::Button *button2 = tab->text_button("Settings");
                button2->clickListener = [=]() {
                    this->state = SETTINGS;
                };
                button2->set_position(0.0f, -0.2f)->set_background(buttonDrawable);
                
                ui.mainTable->add(tab);
            }
            
            
            return true;
        }

        bool update() override {
            ic::UI::Core::get().update(ic::Time::globalDelta);
        
            clear_color(ic::Colors::blue);

            if (state == MENU) {
            } else if (state == PLAYING) {
                controller.act(ic::Time::delta);
                camera.update();
                
                
                shader.use();
                shader.set_uniform_vec3f("viewPosition", camera.position);
                camera.upload_to_shader(shader);
    
    
                ic::GLStateHandler::enable_face_culling(ic::FRONT, ic::CCW);
                floorTexture.use();
                floorMesh.draw(shader);
    
    
                ic::GLStateHandler::disable_face_culling();
                windowTexture.use();
    
                // Inspired from LearnOpenGL.com
                // Sort quads based on their midpoint distance to the camera, in order to prevent overwriting 
                std::map<float, ic::Vec3f> sorted;
                ic::Vec3f xAxis(1.0f, 0.0f, 0.0f);
    
                for (auto &windowPos : windows) {
                    float distance = camera.position.dst2(windowPos);
                    sorted[distance] = windowPos;
                }
    
                for (std::map<float, ic::Vec3f>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); it++) {
                    // Switch normal direction if the camera is facing away from the X axis
                    bool frontFacing = (camera.position - it->second).dot(xAxis) >= 0.0f;
                    ic::Mat4x4 scaling = ic::Mat4x4().set_scaling<3>({ frontFacing ? 1.0f : -1.0f, 1.0f, 1.0f });
    
                    mesh.set_transformation(ic::Mat4x4().set_translation<3>(it->second));
                    mesh.set_normal_transformation(scaling);
                    mesh.draw(shader);
                }
            }
            ic::UI::Core::get().render(ic::Time::globalDelta);

            return true; 
        }

        void dispose() override {
            shader.clear();
            mesh.dispose();
            floorMesh.dispose();
        }
};


int main() {
    UI3D application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}