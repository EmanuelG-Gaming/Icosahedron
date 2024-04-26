#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>

#include <Physics/levels/PhysicsLevel.h>
#include <Physics/objects/RigidObject.h>
#include <IcosahedronDebug/ConsoleOutput.h>

/** @brief Icosahedron's physics engine. */
class BasicPhysics : public ic::Application {
    ic::Mesh2D mesh1, mesh2, mesh3;
    ic::Camera2D camera;
    ic::Shader shader;
    ic::Physics::RigidObject *rigidBody1, *rigidBody2, *rigidBody3;
    ic::Physics::PhysicsLevel level;

    float angle1, angle2;

    public:
        bool init() override {
            engine.window.set_title("Basic physics example");
            
            return true;
        }
        
        bool load() override {
            angle1 = -0.5f;
            angle2 = 0.1f;

            mesh1 = ic::GeometryGenerator::generate_regular_polygon_mesh(20, 0.3f);
            mesh1.set_material(ic::MeshMaterial2D(ic::Colors::yellow, 1.0f));

            auto positions1 = std::vector<float>({
                -2.0f, -0.5f,
                2.0f, -0.5f,
                2.0f, 0.5f,
                -2.0f, 0.5f,
            });
            auto positions2 = std::vector<float>({
                -2.0f, -0.5f,
                2.0f, -0.5f,
                -2.0f, 0.5f,
            });


            mesh2 = ic::GeometryGenerator::generate_mesh(positions1);
            mesh2.set_material(ic::MeshMaterial2D(ic::Colors::green, 1.0f));

            mesh3 = ic::GeometryGenerator::generate_mesh(positions2);
            mesh3.set_material(ic::MeshMaterial2D(ic::Colors::gray, 1.0f));


            shader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex2D, ic::Shaders::meshShaderFrag2D);
            
            camera = ic::Camera2D();
            camera.scale = 0.5f;

            level = ic::Physics::PhysicsLevel();
            level.set_gravity(0.0f, -1.81f);
            level.simulationSteps = 10;

            rigidBody1 = new ic::Physics::RigidObject();
            rigidBody1->collider = new ic::Physics::SphereCollider(0.3f);
            rigidBody1->dynamic = true;
            rigidBody1->set_position(-1.5f, 2.0f);
            rigidBody1->apply_velocity(0.8f, 0.0f);

            level.add_object(rigidBody1);


            rigidBody2 = new ic::Physics::RigidObject();
            rigidBody2->collider = new ic::Physics::PolygonCollider(positions1);

            rigidBody2->dynamic = false;
            rigidBody2->set_position(0.0f, -0.7f);
            rigidBody2->restitution = 0.1f;
            rigidBody2->transform->rotation = rigidBody2->transform->rotation.from_euler(0.0f, 0.0f, angle1);
            
            level.add_object(rigidBody2);


            rigidBody3 = new ic::Physics::RigidObject();
            rigidBody3->collider = new ic::Physics::PolygonCollider(positions2);

            rigidBody3->dynamic = false;
            rigidBody3->set_position(-0.1f, -1.2f);
            rigidBody3->restitution = 0.1f;
            rigidBody3->transform->rotation = rigidBody2->transform->rotation.from_euler(0.0f, 0.0f, angle2);
            
            level.add_object(rigidBody3);

            // Retry
            ic::KeyboardController *kb = new ic::KeyboardController();
            kb->add_key_up_action([=]() {
                rigidBody1->set_position(-1.5f, 2.0f);
                rigidBody1->velocity = ic::Vec3(0.8f, 0.0f, 0.0f);
            }, KEY_Q);
            ic::InputHandler::add_input(kb, "keyboard");


            return true;
        }

        bool update() override {
            
            level.update(ic::Time::delta);
            
            mesh1.set_transformation(ic::Mat4x4().set_translation<3>(rigidBody1->transform->position));
            mesh2.set_transformation(ic::Mat4x4().set_translation<3>(rigidBody2->transform->position) * rigidBody2->transform->rotation.to_rotation_matrix());
            mesh3.set_transformation(ic::Mat4x4().set_translation<3>(rigidBody3->transform->position) * rigidBody3->transform->rotation.to_rotation_matrix());

            ic::GL::clear_color(ic::Colors::blue);

            shader.use();
            camera.use(shader);
            
            mesh1.draw(shader);
            mesh2.draw(shader);
            mesh3.draw(shader);

            return true; 
        }

        void dispose() override {
            shader.clear();
            mesh1.dispose();
            mesh2.dispose();
            mesh3.dispose();
        }
};

int main() {
    ic::Debug::create_console();

    BasicPhysics application;

    application.construct(640, 480);
    application.start();

    return 0;
}