#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/3d/Camera3D.h>
#include <Icosahedron/scene/3d/Mesh3D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>

#include <Icosahedron/scene/3d/controllers/FreeRoamCameraController3D.h>

#include <Physics/3d/objects/RigidObject3D.h>
#include <Physics/3d/levels/PhysicsLevel3D.h>
#include <Physics/3d/forces/WindForce3D.h>


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
        vec3(-8.0, -5.0, -2.0), 
        vec3(0.2, 0.2, 0.2), 
        vec3(0.8, 0.8, 0.8), 
        vec3(0.5, 0.5, 0.5)
    );
    
    uniform vec3 diffuseColor = vec3(1.0, 1.0, 1.0);

    uniform vec3 viewPosition;
    out vec4 outColor;

    vec4 compute_lighting(DirectionalLight light) {
        vec3 normal = normalize(vNormal);
        vec3 lightDirection = normalize(-light.direction);
        vec3 viewDirection = normalize(viewPosition - vPosition);
        float dotProduct = dot(lightDirection, normal);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4;
        vec4 ambientColor = vec4(light.ambient, 1.0) * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0);
        vec4 diffuseColor = vec4(diffuseColor * light.diffuse, 1.0) * diffuseIntensity;

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0);
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0);
        
        vec4 specularColor = vec4(light.specular, 1.0) * specularIntensity;
        vec4 result = ambientColor + diffuseColor + specularColor;
        return result;
    }
    
    void main() {
        vec4 color = compute_lighting(l);
        outColor = color;
    }
);


struct Object {
    ic::Mesh3D mesh;
    ic::Physics::RigidObject3D *rigidBody;
    ic::Color color;
};


class Physics3D: public ic::Application {
    ic::Color floorColor;
    ic::Shader shader;

    ic::Camera3D camera;
    std::vector<Object*> objects;

    ic::Physics::PhysicsLevel3D level;
    ic::FreeRoamCameraController3D controller;
    ic::Physics::WindForce3D blowForce;

    ic::KeyboardController *keyboard;
    bool blowingAir;

    public:
        bool init() override {
            displayName = "Physics in 3D";
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            ic::GLStateHandler::enable_depth_testing(ic::LESS);
            
            shader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex3D, fragment);


            std::vector<float> positions = {
                -5.0f, 0.0f, -5.0f,
                5.0f, 0.0f, -5.0f,
                5.0f, 0.0f, 5.0f,
                -5.0f, 0.0f, 5.0f, 
            };

            std::vector<float> normals = {
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
            };


            camera = ic::Camera3D();
            camera.position = { -8.0f, 4.0f, 4.0f };
            camera.lookingAt = { 0.0f, 0.5f, 0.0f };

            controller = ic::FreeRoamCameraController3D(&camera);
            controller.flying = true;

            floorColor = ic::Color().hexadecimal_to_RGB("268B07");
            
            level = ic::Physics::PhysicsLevel3D();
            level.set_gravity(0.0f, -0.981f, 0.0f);
            level.simulationSteps = 10;

            for (int i = 0; i < 100; i++) {
                ic::Physics::RigidObject3D *rigidBody = new ic::Physics::RigidObject3D();
                rigidBody->collider = new ic::Physics::SphereCollider(0.3f);
                rigidBody->dynamic = true;
                rigidBody->set_position(-0.5f + rand() % 300 / 300.0f, -5.0f + 0.3f + 0.9f + i * 0.6f, 0.0f + rand() % 300 / 300.0f);
                rigidBody->apply_velocity(0.0f, 1.0f, 0.0f);

                level.add_object(rigidBody);
                objects.push_back(new Object({ ic::GeometryGenerator::generate_UV_sphere_mesh(0.3f, 12, 12), rigidBody, ic::Colors::lightGray }));
            }

            {
                ic::Physics::RigidObject3D *rigidBody = new ic::Physics::RigidObject3D();
                rigidBody->collider = new ic::Physics::BoxCollider(10.0f, 0.9f, 10.0f);
                rigidBody->dynamic = false;
                rigidBody->set_position(0.0f, -5.0f, 0.0f);
                rigidBody->restitution = 0.1f;
            
                level.add_object(rigidBody);
                objects.push_back(new Object({ ic::GeometryGenerator::generate_parallelipiped_mesh(10.0f, 0.9f, 10.0f), rigidBody, floorColor }));
            }

            {
                ic::Physics::RigidObject3D *rigidBody = new ic::Physics::RigidObject3D();
                rigidBody->collider = new ic::Physics::BoxCollider(3.0f, 0.1f, 3.0f);
                rigidBody->dynamic = false;
                rigidBody->set_position(0.0f, -3.0f, 0.0f);
                rigidBody->restitution = 0.9f;
                rigidBody->transform->rotation = ic::Quaternion().from_euler(0.0f, 1.0f, 2.0f);

                level.add_object(rigidBody);
                objects.push_back(new Object({ ic::GeometryGenerator::generate_parallelipiped_mesh(3.0f, 0.1f, 3.0f), rigidBody, ic::Colors::blue }));
            }


            blowForce = ic::Physics::WindForce3D(0.1f, 0.1f);

            keyboard = new ic::KeyboardController();
            keyboard->add_key_down_action([this]() { blowingAir = !blowingAir; }, KEY_Q);
            ic::InputHandler::add_input(keyboard, "key");


            blowingAir = false;
           

            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            controller.act(dt);

            level.reset_forces();
            if (blowingAir) {
                ic::Vec3f to = (camera.lookingAt - camera.position);
                for (auto &obj : objects) {
                    blowForce.force = to / camera.position.dst2(obj->rigidBody->transform->position);
                    blowForce.apply(obj->rigidBody);
                }
            }

            level.update(dt);

            camera.update();
            

            clear_color(ic::Colors::blue);
            
            shader.use();
            shader.set_uniform_vec3f("viewPosition", camera.position);
            camera.upload_to_shader(shader);

            for (auto &obj : objects) {
                ic::Mat4x4 rotation = obj->rigidBody->transform->rotation.to_rotation_matrix();
                obj->mesh.set_transformation(ic::Mat4x4().set_translation<3>(obj->rigidBody->transform->position) * rotation);
                obj->mesh.set_normal_transformation(rotation);

                shader.set_uniform_color("diffuseColor", obj->color);
                obj->mesh.draw(shader);
            }

            return true; 
        }

        void dispose() override {
            shader.clear();
        }
};


int main() {
    Physics3D application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}