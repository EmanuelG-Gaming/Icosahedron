#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>

#include <Physics/2d/levels/PhysicsLevel2D.h>
#include <Physics/2d/objects/RigidObject2D.h>


class PhysicsDebug : public ic::Application {
    ic::Batch textBatch;
    ic::TextAtlas atlas;

    ic::Mesh2D mesh2;
    ic::Camera2D camera, uiCamera;
    ic::Shader shader, textShader;
    ic::Physics::RigidObject2D *rigidBody2;
    ic::Physics::PhysicsLevel2D level;

    std::vector<ic::Mesh2D> meshes;
    
    float kineticEnergy;

    public:
        bool init() override {
            displayName = "Basic physics example";
            
            return true;
        }
        
        bool load() override {
            mesh2 = ic::GeometryGenerator::get().generate_regular_polygon_mesh(30, 0.5f);
            mesh2.set_material(ic::MeshMaterial2D(ic::Colors::green, 1.0f));


            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D);
            textShader = ic::ShaderLoader::get().load(shaders.basicTextShaderVertex2D, shaders.basicTextShaderFrag2D);
            
            textBatch = ic::Batch(10000, ic::TRIANGLES);
            ic::FreeType::get().add_atlas("score", "resources/fonts/Roboto-Regular.ttf", 48);
            atlas = ic::FreeType::get().find_atlas("score");
             
            
            camera = ic::Camera2D();
            camera.scale = 0.5f;
            uiCamera = ic::Camera2D();



            level = ic::Physics::PhysicsLevel2D();
            level.simulationSteps = 10;
            level.set_gravity(0.0f, 0.0f);
            
            rigidBody2 = new ic::Physics::RigidObject2D();
            rigidBody2->collider = new ic::Physics::CircleCollider(0.5f);
            rigidBody2->dynamic = false;
            rigidBody2->set_position(0.0f, 0.0f);
            
            level.add_object(rigidBody2);
            meshes.push_back(mesh2);

            kineticEnergy = 0.0f;

            ic::KeyboardController *keys = new ic::KeyboardController();
            keys->add_key_up_action([this]() { add_object(-3.0f, 0.0f, 5.0f, 0.01f ); }, KEY_P);
            ic::InputHandler::get().add_input(keys, "Keys");

            return true;
        }


        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            level.update(dt);
            kineticEnergy = 0.0f;

            for (auto &object : level.get_objects()) {
                ic::Physics::RigidObject2D *body = dynamic_cast<ic::Physics::RigidObject2D*>(object);
                if (body == nullptr) continue;

                // Calculate kinetic energy based on the formula mv^2 / 2
                kineticEnergy += (body->mass * body->velocity.len2()) / 2.0f;
            }

            int size = level.get_objects().size();

            mesh2.set_transformation(ic::Mat4x4().set_translation<2>(rigidBody2->transform->position));

            int i = 0;
            for (auto &mesh : meshes) {
                mesh.set_transformation(ic::Mat4x4().set_translation<2>(level.get_object(i)->transform->position));
                i++;
            }
            
            clear_color(ic::Colors::blue);

            shader.use();
            camera.use(shader);
            
            for (auto &mesh : meshes) {
                mesh.draw(shader);
            }


            textShader.use();
            uiCamera.use(textShader);
            atlas.use();

            renderer.draw_string(textBatch, atlas, "Object count: " + std::to_string(size), -1.2f, 0.9f);
            renderer.draw_string(textBatch, atlas, "Total kinetic energy: " + std::to_string(kineticEnergy) + " J", -1.2f, 0.8f);
           
            textBatch.render();


            return true; 
        }

        void add_object(float x, float y, float velX, float velY) {
            float radius = rand() % 50 / 200.0f + 0.01f;
            
            ic::Mesh2D mesh = ic::GeometryGenerator::get().generate_rectangle_mesh(radius, radius);
            mesh.set_material(ic::MeshMaterial2D(ic::Colors::lightGray, 1.0f));

            ic::Physics::RigidObject2D *rigidBody = new ic::Physics::RigidObject2D();
            rigidBody->collider = new ic::Physics::RectangleCollider(radius, radius);
            rigidBody->dynamic = true;
            rigidBody->set_mass(radius * 5.0f);
            rigidBody->set_position(x, y);
            rigidBody->apply_velocity(velX, velY);
            
            level.add_object(rigidBody);
            meshes.push_back(mesh);
        }

        void dispose() override {
            shader.clear();
            mesh2.dispose();

            for (auto &mesh : meshes) {
                mesh.dispose();
            }
        }
};

int main() {
    PhysicsDebug application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}