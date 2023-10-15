#include <Icosahedron/Core.h>

#include <Physics/2d/levels/PhysicsLevel2D.h>
#include <Physics/2d/objects/RigidObject2D.h>


class PiCounting : public ic::Application {
    ic::Batch *textBatch;
    ic::TextAtlas *atlas;
    ic::Texture *floorTexture, *wallTexture;
    ic::Texture *piTexture;

    ic::Mesh2D *wallMesh, *floorMesh, *movingMesh, *rigidMesh, *piMesh;
    ic::Camera2D *camera, *uiCamera;
    ic::Shader *shader, *textShader;
    ic::Physics::PhysicsLevel2D *level;
    ic::Physics::RigidObject2D *floor, *wall, *moving, *rigidBody;

    int collisions;
    bool spectatingMoving;

    public:
        bool init() override {
            displayName = "Basic physics example";
            scaling = ic::WindowScaling::fullscreen;

            return true;
        }
        
        bool load() override {
            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D);
            textShader = ic::ShaderLoader::get().load(shaders.basicTextShaderVertex2D, shaders.basicTextShaderFrag2D);
            
            textBatch = new ic::Batch(10000, ic::TRIANGLES);
            ic::FreeType::get().add_atlas("score", "resources/fonts/Roboto-Regular.ttf", 48);
            atlas = ic::FreeType::get().find_atlas("score");
            
            piTexture = ic::TextureLoader::get().load_png("resources/textures/pi.png");
            piMesh = ic::GeometryGenerator::get().generate_rectangle_mesh(0.08f, 0.08f);
            piMesh->set_transformation(ic::Mat4x4().set_translation<2>({ 0.5f, 0.7f }));

            floorTexture = ic::TextureLoader::get().load_png("resources/textures/wood.png");
            wallTexture = ic::TextureLoader::get().load_png("resources/textures/stone-bricks.png");

            camera = new ic::Camera2D();
            camera->scale = 0.5f;
            uiCamera = new ic::Camera2D();

            wallMesh = ic::GeometryGenerator::get().generate_rectangle_mesh(0.3f, 0.5f, 1.0f, 2.0f);
            wallMesh->set_material(ic::MeshMaterial2D(ic::Colors::white, 1.0f));
            
            floorMesh = ic::GeometryGenerator::get().generate_rectangle_mesh(200.0f, 0.3f, 200.0f, 0.3f);
            floorMesh->set_material(ic::MeshMaterial2D(ic::Colors::white, 1.0f));

            movingMesh = ic::GeometryGenerator::get().generate_regular_polygon_mesh(8, 0.2f);
            movingMesh->set_material(ic::MeshMaterial2D(ic::Colors::yellow, 1.0f));
            
            rigidMesh = ic::GeometryGenerator::get().generate_regular_polygon_mesh(8, 0.2f);
            rigidMesh->set_material(ic::MeshMaterial2D(ic::Colors::cyan, 1.0f));
            

            level = new ic::Physics::PhysicsLevel2D();
            level->set_gravity(0.0f, -9.81f);
            level->set_fixed_time_length(60);
            level->simulationSteps = 1000;

            wall = new ic::Physics::RigidObject2D();
            wall->collider = new ic::Physics::RectangleCollider(0.3f, 0.5f);
            wall->dynamic = false;
            wall->set_position(0.3f, 0.0f);
            level->add_object(wall);
            

            
            floor = new ic::Physics::RigidObject2D();
            floor->collider = new ic::Physics::RectangleCollider(200.0f, 0.3f);
            floor->dynamic = false;
            floor->set_position(200.0f, -0.8f);
            level->add_object(floor);
            

            
            moving = new ic::Physics::RigidObject2D();
            moving->collider = new ic::Physics::CircleCollider(0.2f);
            moving->dynamic = true;
            moving->set_mass(100.0f);
            moving->set_position(5.0f, -0.3f + 0.2f);
            moving->apply_velocity(-1.0f, 0.0f);
        
            level->add_object(moving);
            

            
            rigidBody = new ic::Physics::RigidObject2D();
            rigidBody->collider = new ic::Physics::CircleCollider(0.2f);
            rigidBody->dynamic = true;
            rigidBody->set_position(3.0f, -0.3f);
            rigidBody->apply_velocity(0.0f, 0.0f);
            
            rigidBody->collision([&](ic::Physics::Manifold2D manifold, float dt) {
                if ((manifold.object1 == wall || manifold.object2 == wall) ||
                    (manifold.object1 == moving || manifold.object2 == moving)) {
                    collisions++;
                }
            });
    
            level->add_object(rigidBody);
            

            collisions = 0;
            spectatingMoving = true;

            ic::KeyboardController *keys = new ic::KeyboardController();
            keys->add_key_up_action([this]() { spectatingMoving = !spectatingMoving; }, KEY_Q);
            inputHandler.add_input(keys, "Keys");

            return true;
        }

        void window_size_changed(int w, int h) override {
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            level->update(dt);

            wallMesh->set_transformation(ic::Mat4x4().set_translation<2>(wall->transform->position));
            floorMesh->set_transformation(ic::Mat4x4().set_translation<2>(floor->transform->position));
            rigidMesh->set_transformation(ic::Mat4x4().set_translation<2>(rigidBody->transform->position));
            movingMesh->set_transformation(ic::Mat4x4().set_translation<2>(moving->transform->position));
            
            camera->position = spectatingMoving ? moving->transform->position : rigidBody->transform->position;

            clear_color(ic::Colors::blue);

            shader->use();
            camera->use(shader);
            
            wallTexture->use();
            wallMesh->draw(shader);
            wallTexture->unuse();

            floorTexture->use();
            floorMesh->draw(shader);
            floorTexture->unuse();

            rigidMesh->draw(shader);
            movingMesh->draw(shader);      


            textShader->use();
            uiCamera->use(textShader);
            atlas->use();

            renderer.draw_string(textBatch, atlas, " = 3.14159265...", 0.55f, 0.65f);
            renderer.draw_string(textBatch, atlas, "Collisions: " + std::to_string(collisions), 0.4f, 0.85f);

            textBatch->render();


            shader->use();
            uiCamera->use(shader);
            piTexture->use();

            piMesh->draw(shader);

            piTexture->unuse();


            return true; 
        }

        void dispose() override {
            shader->clear();
            textShader->clear();
        }
};

int main(int argc, char *argv[]) {
    PiCounting application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}