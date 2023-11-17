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

#include <Physics/2d/levels/PhysicsLevel2D.h>
#include <Physics/2d/objects/RigidObject2D.h>


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
    uniform sampler2D bloomTexture;

    out vec4 outColor;

    void main() {
        vec3 hdrColor = texture(screenTexture, vTCoords).rgb;
        hdrColor += texture(bloomTexture, vTCoords).rgb;
        
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

std::string blurShaderFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;

    in vec2 vPosition;
    in vec2 vTCoords;

    uniform bool horizontal;
    uniform float weight[5] = float[](0.227, 0.194, 0.121, 0.054, 0.016);

    uniform sampler2D image;

    out vec4 outColor;

    void main() {
        vec2 textureOffset = 1.0 / textureSize(image, 0);
        vec3 result = texture(image, vTCoords).rgb * weight[0];

        if (horizontal) {
            for (int i = 1; i < 5; i++) {
              result.rgb += texture(image, vTCoords + vec2(textureOffset.x * i, 0.0)).rgb * weight[i];
              result.rgb += texture(image, vTCoords - vec2(textureOffset.x * i, 0.0)).rgb * weight[i];
            }
        }
        else {
            for (int i = 1; i < 5; i++) {
                result.rgb += texture(image, vTCoords + vec2(0.0, textureOffset.y * i)).rgb * weight[i];
                result.rgb += texture(image, vTCoords - vec2(0.0, textureOffset.y * i)).rgb * weight[i];
            }
        }

        
        outColor = vec4(result, 1.0);
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

    struct DirectionalLight {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

    PointLight l = PointLight(
        vec3(-2.0, 3.0, -3.0), 
        vec3(0.1, 0.1, 0.1), 
        vec3(5.0, 5.0, 5.0), 
        vec3(2.0, 2.0, 2.0), 
        
        0.01, 0.05, 0.01
    );

    PointLight l1 = PointLight(
        vec3(10.0, 3.0, 30.0), 
        vec3(0.1, 0.1, 0.1), 
        vec3(5.0, 5.0, 5.0), 
        vec3(2.0, 2.0, 2.0), 
        
        0.01, 0.05, 0.01
    );
    PointLight l2 = PointLight(
        vec3(100.0, 3.0, 20.0), 
        vec3(0.1, 0.1, 0.1), 
        vec3(5.0, 5.0, 5.0), 
        vec3(2.0, 2.0, 2.0), 
        
        0.01, 0.05, 0.01
    );
    
    DirectionalLight d = DirectionalLight(
        vec3(-1.0, -5.0, -2.0), 
        vec3(0.4, 0.4, 0.4), 
        vec3(5.0, 5.0, 5.0), 
        vec3(0.1, 0.1, 0.1)
    );

    const float BLOOM_THRESHOLD = 5.0f;

    uniform sampler2D sampleTexture;
    uniform vec3 viewPosition;

    layout (location = 0) out vec4 outColor;
    layout (location = 1) out vec4 outBrightColor;

    vec3 compute_lighting(PointLight light) {
        float distance = length(light.position - vPosition);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        vec3 normal = normalize(vNormal);
        vec3 lightDirection = normalize(light.position - vPosition);
        vec3 viewDirection = normalize(viewPosition - vPosition);
        float dotProduct = clamp(dot(lightDirection, normal), 0.0, 1.0);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4 * attenuation;
        vec3 ambientColor = light.ambient * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0) * attenuation;
        vec3 diffuseColor = texture(sampleTexture, vTCoords).rgb * light.diffuse * diffuseIntensity;

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        vec3 specularColor = light.specular * ambientIntensity;
        vec3 result = ambientColor + diffuseColor + specularColor;
        return result;
    }

    vec3 compute_lighting(DirectionalLight light) {
        vec3 normal = normalize(vNormal);
        vec3 lightDirection = normalize(-light.direction);
        vec3 viewDirection = normalize(viewPosition - vPosition);
        float dotProduct = dot(lightDirection, normal);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4;
        vec3 ambientColor = light.ambient * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0);
        vec3 diffuseColor = texture(sampleTexture, vTCoords).rgb * light.diffuse * diffuseIntensity;

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0);
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0);
        
        vec3 specularColor = light.specular * specularIntensity;
        vec3 result = ambientColor + diffuseColor + specularColor;
        return result;
    }
    
    void main() {
        vec3 color = compute_lighting(l) + compute_lighting(l1) + compute_lighting(l2) + compute_lighting(d);
        float brightness = dot(color, vec3(0.216, 0.715, 0.072));

        outColor = vec4(color, 1.0);
        
        if (brightness > BLOOM_THRESHOLD) {
            outBrightColor = vec4(outColor.rgb, 1.0);
        } else {
            outBrightColor = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }
);

struct Object {
    ic::Physics::RigidObject2D *rigid;
    ic::Mesh3D *mesh;
    ic::Texture *texture;
};

class TopDown3D : public ic::Application {
    std::vector<Object*> objects;
    Object *ball;

    ic::Shader *shader, *blurShader, *screenShader;
    ic::Framebuffer *sceneFramebuffer;
    ic::Framebuffer *pingpong1, *pingpong2;

    ic::Camera3D *camera;
    ic::Mesh2D *screenQuad;
    ic::Mesh3D *floorMesh;

    ic::Texture *floorTexture, *whiteTexture;
    ic::FreeRoamCameraController3D *controller;
    
    ic::Physics::PhysicsLevel2D *level;

    float time;
    float exposure;

    public:
        bool init() override {
            displayName = "Top-down 3D";
           
            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            //states.enable_face_culling(ic::FRONT, ic::CCW);
            


            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex3D, fragment);
            blurShader = ic::ShaderLoader::get().load(screenVertex, blurShaderFragment);
            screenShader = ic::ShaderLoader::get().load(screenVertex, screenFragment);
            


            // Shader configuration
            shader->use();
            shader->set_uniform_int("sampleTexture", 0);

            blurShader->use();
            blurShader->set_uniform_int("image", 0);

            screenShader->use();
            screenShader->set_uniform_int("screenTexture", 0);
            screenShader->set_uniform_int("bloomTexture", 1);




            ic::TextureParameters params;
            params.usesMipmapping = true;

            floorTexture = ic::TextureLoader::get().load_png("resources/textures/wood.png", params, true);
            whiteTexture = ic::TextureLoader::get().load_png("resources/textures/white.png", params, true);
            
            sceneFramebuffer = new ic::Framebuffer(ic::TEXTURE_ATTACH_COLOR_0, ic::TEXTURE_RGBA_16F, ic::TEXTURE_RGBA, IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            sceneFramebuffer->add_render_target(ic::TEXTURE_ATTACH_COLOR_1, ic::TEXTURE_RGBA_16F, ic::TEXTURE_RGBA);

            pingpong1 = pingpong2 = new ic::Framebuffer(ic::TEXTURE_ATTACH_COLOR_0, ic::TEXTURE_RGBA, ic::TEXTURE_RGBA, IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT, false);


            floorMesh = ic::GeometryGenerator::get().generate_parallelipiped_mesh(2000.0f, 0.1f, 2000.0f, 4000.0f, 0.2f, 4000.0f);
            
            screenQuad = new ic::Mesh2D();
            screenQuad->add_attribute(0, 2, ic::GeometryGenerator::get().generate_rectangle(1.0f, 1.0f));
            screenQuad->add_attribute(1, 2, ic::GeometryGenerator::get().generate_UV_rectangle(1.0f, 1.0f));
            screenQuad->set_index_buffer({ 0, 1, 2, 0, 2, 3 });

            
            camera = new ic::Camera3D();
            camera->position = { -3.0f, 1.5f, 0.0f };
            controller = new ic::FreeRoamCameraController3D(camera, &ic::InputHandler::get());
            controller->flying = true;


            // Controls
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

            ic::InputHandler::get().add_input((new ic::KeyboardController())->with_WASD(), "WASD");


            time = 0.0f;
            exposure = 0.05;



            level = new ic::Physics::PhysicsLevel2D();
            level->set_gravity(0.0f, 0.0f);
            level->simulationSteps = 10;

            ball = new Object();
            ball->mesh = ic::GeometryGenerator::get().generate_UV_sphere_mesh(0.5f, 12, 12);
            ball->texture = ic::TextureLoader::get().load_png("resources/textures/white.png", params, true);

            ball->rigid = new ic::Physics::RigidObject2D();
            ball->rigid->collider = new ic::Physics::CircleCollider(0.5f);
            ball->rigid->dynamic = true;
            ball->rigid->set_position(0.0f, 0.0f);
            
            level->add_object(ball->rigid);
            objects.push_back(ball);



            for (int i = 0; i < 250; i++) {
                float x = (rand() % 500 - 500.0f / 2.0f);
                float z = (rand() % 500 - 500.0f / 2.0f);
                
                float width = (rand() % 100 / 50.0f) + 0.1f;
                float height = (rand() % 100 / 50.0f) + 0.1f;

                Object *wall3 = new Object();
                wall3->mesh = new ic::Mesh3D();

                std::vector<float> positions = {
                    -2.5f, -0.1f, -0.5f,
                    0.0f,  -0.1f, 1.0f,
                    0.5f,  -0.1f, -0.5f
                };
                wall3->mesh->add_attribute(0, 3, positions);
                wall3->mesh->set_index_buffer({0, 1, 2});
    
                //wall3->mesh = ic::GeometryGenerator::get().generate_parallelipiped_mesh(width, 1.0f, height, width * 2.0f, 2.0f, height * 2.0f);
                wall3->texture = floorTexture;

                wall3->rigid = new ic::Physics::RigidObject2D();
                //wall3->rigid->collider = new ic::Physics::RectangleCollider(width, height);
                wall3->rigid->collider = new ic::Physics::PolygonCollider({
                    -2.5f, -0.5f,
                    0.5f, -0.5f,
                    0.0f, 1.0f,
                });

                wall3->rigid->dynamic = false;
                wall3->rigid->restitution = 0.1f;
                wall3->rigid->dynamicFriction = 0.5f;
                wall3->rigid->staticFriction = 0.5f;
                wall3->rigid->set_position(x, z);
                
                level->add_object(wall3->rigid);
                objects.push_back(wall3);
            }

            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        void window_size_changed(int w, int h) override {
            camera->resize(w, h);

            sceneFramebuffer->resize(w, h);
            pingpong1->resize(w, h);
            pingpong2->resize(w, h);
        }

        bool update(float dt) override {
            time += dt;

            level->update(dt);

            auto *controller = ic::InputHandler::get().find_keyboard("WASD");
            ic::Vec2i dir = controller->get_direction();

            float speed = 20.0f;
            ball->rigid->velocity.x() += dir.x() * speed * dt;
            ball->rigid->velocity.y() += dir.y() * speed * dt;

            camera->position = { ball->rigid->transform->position.x(), 10.0f, ball->rigid->transform->position.y() - 0.01f };
            camera->lookingAt = { ball->rigid->transform->position.x(), 0.6f, ball->rigid->transform->position.y() };

            camera->update();
            
            clear_color(ic::Colors::blue);
            
            // First pass - scene
            sceneFramebuffer->use();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader->use();
            shader->set_uniform_vec3f("viewPosition", camera->position);
            camera->upload_to_shader(shader);
            

            for (auto &object : objects) {
                object->mesh->set_transformation(ic::Mat4x4().set_translation<3>({ object->rigid->transform->position.x(), 0.6f, object->rigid->transform->position.y() }));
    
                object->texture->use();
                object->mesh->draw(shader);
            }


            floorTexture->use();
            floorMesh->draw(shader);

            sceneFramebuffer->unuse();

            // Second pass - blurring via the two ping pong framebuffers
            bool horizontal = true, firstIteration = true;
            int amount = 8;

            blurShader->use();
            for (int i = 0; i < amount; i++) {
                ic::Framebuffer *pingpong = horizontal ? pingpong1 : pingpong2;

                pingpong->use();
                blurShader->set_uniform_bool("horizontal", horizontal);
            
                if (firstIteration) {
                    sceneFramebuffer->use_texture(0, 1);
                } else {
                    ic::Framebuffer *pingpongOpposite = horizontal ? pingpong2 : pingpong1;
                    pingpongOpposite->use_texture(0, 0);
                }
                screenQuad->draw(blurShader);
            
                horizontal = !horizontal;
                if (firstIteration) {
                    firstIteration = false;
                }
            }
            pingpong1->unuse();
            pingpong2->unuse();
            sceneFramebuffer->unuse();
            
            // Third pass - Add the blurred texture to the model screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            screenShader->use();
            screenShader->set_uniform_float("exposure", exposure);

            sceneFramebuffer->use_texture(0, 0);
            (horizontal ? pingpong2 : pingpong1)->use_texture(1, 0);

            screenQuad->draw(screenShader);

            return true; 
        }

        void dispose() override {
            shader->clear();
            floorMesh->dispose();
            floorTexture->dispose();
            whiteTexture->dispose();
            sceneFramebuffer->dispose();
        }
};


int main(int argc, char *argv[]) {
    TopDown3D application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}