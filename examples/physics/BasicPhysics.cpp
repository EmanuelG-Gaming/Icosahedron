#include <Icosahedron/Core.h>

#include <Physics/2d/levels/PhysicsLevel2D.h>
#include <Physics/2d/objects/RigidObject2D.h>


/** @brief Icosahedron's physics engine. */
class BasicPhysics : public ic::Application {
    ic::Mesh2D *mesh1, *mesh2;
    ic::Camera2D *camera;
    ic::Shader *shader;
    ic::Physics::RigidObject2D *rigidBody1, *rigidBody2;
    ic::Physics::PhysicsLevel2D *level;

    float time;

    public:
        bool init() override {
            displayName = "Basic physics example";

            return true;
        }
        
        bool load() override {
            mesh1 = ic::GeometryGenerator::get().generate_regular_polygon_mesh(5, 0.3f);
            mesh1->set_material(ic::MeshMaterial2D(ic::Colors::yellow, 1.0f));

            mesh2 = ic::GeometryGenerator::get().generate_regular_polygon_mesh(3, 0.3f);
            mesh2->set_material(ic::MeshMaterial2D(ic::Colors::green, 1.0f));

            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D);
            
            camera = new ic::Camera2D();
            camera->scale = 0.5f;

            time = 0.0f;


            level = new ic::Physics::PhysicsLevel2D();
            level->load();

            rigidBody1 = new ic::Physics::RigidObject2D();
            rigidBody1->collider = new ic::Physics::CircleCollider(0.3f);
            rigidBody1->dynamic = true;
            rigidBody1->apply_velocity(2.0f, 5.0f);
            level->add_object(rigidBody1);


            rigidBody2 = new ic::Physics::RigidObject2D();
            rigidBody2->collider = new ic::Physics::CircleCollider(0.3f);
            rigidBody2->dynamic = false;
            rigidBody2->transform->position = { 1.7f, -0.5f };
            level->add_object(rigidBody2);


            return true;
        }

        void window_size_changed(int w, int h) override {
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            level->update(dt);

            time += dt;
            mesh1->set_transformation(ic::Mat4x4().set_translation<2>(rigidBody1->transform->position));
            mesh2->set_transformation(ic::Mat4x4().set_translation<2>(rigidBody2->transform->position));
            
            clear_color(ic::Colors::blue);

            shader->use();
            camera->use(shader);
            //mesh1->draw(shader, ic::LINE_STRIPS);
            mesh1->draw(shader);
            mesh2->draw(shader);

            return true; 
        }

        void dispose() override {
            shader->clear();
            mesh1->dispose();
            mesh2->dispose();
        }
};

int main(int argc, char *argv[]) {
    BasicPhysics application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}