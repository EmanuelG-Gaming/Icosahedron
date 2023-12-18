#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Texture.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>

#include <Physics/2d/levels/PhysicsLevel2D.h>
#include <Physics/2d/objects/RigidObject2D.h>

#include <Icosahedron/entity/Entity.h>
#include <Icosahedron/entity/Entities.h>



struct TransformComp : public ic::Component {
    ic::Vec2f position;

    TransformComp() {}
    TransformComp(float x, float y) : position({ x, y }) {}
};

struct SpriteComp : public ic::Component {
    ic::Mesh2D mesh;
    ic::Texture texture;

    SpriteComp() {}
    SpriteComp(ic::Mesh2D mesh, ic::Texture texture) : mesh(mesh), texture(texture) {} 
};

struct MovementComp : public ic::Component {
    float speed = 1.0f;
    bool horizontal = false;

    MovementComp() {}
    MovementComp(float speed, bool horizontal) : speed(speed), horizontal(horizontal) {}
};

struct JumpComp : public ic::Component {
    float jumpSpeed = 5.0f;

    JumpComp() {}
    JumpComp(float jumpSpeed) : jumpSpeed(jumpSpeed) {}
};

struct PhysicsComp : public ic::Component {
    ic::Physics::RigidObject2D *rigidBody;

    PhysicsComp() {}
    PhysicsComp(ic::Physics::Collider2D *collider, ic::Vec2f position, float angle = 0.0f, float mass = 1.0f, bool dynamic = true) {
        this->rigidBody = new ic::Physics::RigidObject2D();

        this->rigidBody->collider = collider;
        this->rigidBody->mass = mass;
        this->rigidBody->transform->angle = angle;
        this->rigidBody->dynamic = dynamic;
        this->rigidBody->restitution = 0.0f;
        this->rigidBody->set_position(position);
    }
};

struct CameraFollowComp : public ic::Component {
    ic::Camera2D *camera;

    CameraFollowComp(ic::Camera2D *camera) : camera(camera) {} 
};


struct SpriteSystem {
    ic::Camera2D camera;

    void init() {
        camera = ic::Camera2D(0.1f);
    }

    void update(ic::Entities &registry, float dt) {
        for (int i = 0; i < ic::lastEntityIndex; i++) {
            ic::Entity *entity = registry.get_entity(i);

            if (entity->has<TransformComp>()) {
                auto &transform = entity->get<TransformComp>();

                if (entity->has<SpriteComp>()) {
                    // Matrices
                    ic::Mat4x4 translation = ic::Mat4x4().set_translation<2>(transform.position);
    
                    entity->get<SpriteComp>().mesh.set_transformation(translation);
                }
            }
        }
    }

    void draw(ic::Entities &registry, ic::Shader &shader) {
        camera.use(shader);

        for (int i = 0; i < ic::lastEntityIndex; i++) {
            ic::Entity *entity = registry.get_entity(i);

            if (entity->has<SpriteComp>()) {
                auto &sprite = entity->get<SpriteComp>();

                sprite.texture.use();
                sprite.mesh.draw(shader);
            }
        }
    }
};

struct MovementSystem {
    ic::KeyboardController *keyboard;

    void load() {
        keyboard = (new ic::KeyboardController())->with_WASD();
        ic::InputHandler::get().add_input(keyboard, "WASD");
    }



    void handle_event(ic::Entities &registry, ic::Event &event, float dt) {
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            for (int i = 0; i < ic::lastEntityIndex; i++) {
                ic::Entity *entity = registry.get_entity(i);
        
                if (entity->has<PhysicsComp>() && entity->has<JumpComp>()) {
                    auto *rigid = entity->get<PhysicsComp>().rigidBody;
                    
                    rigid->apply_velocity(0.0f, entity->get<JumpComp>().jumpSpeed);
                }
            }
        }
    }

    void update(ic::Entities &registry, float dt) {
        ic::Vec2f dir = keyboard->get_direction().as<float>();
        
        for (int i = 0; i < ic::lastEntityIndex; i++) {
            ic::Entity *entity = registry.get_entity(i);

            if (entity->has<TransformComp>() && entity->has<PhysicsComp>() && entity->has<MovementComp>()) {
                auto *physics = entity->get<PhysicsComp>().rigidBody;
                float deltaSpeed = entity->get<MovementComp>().speed * dt;

                // Currently move all entities with MovementComp in the direction of the controller
                physics->velocity.x() += dir.x() * deltaSpeed;
                if (!entity->get<MovementComp>().horizontal) physics->velocity.y() += dir.y() * deltaSpeed;

                physics->velocity += dir * deltaSpeed;
            }
        }
    }
};

struct PhysicsSystem {
    ic::Physics::PhysicsLevel2D level;

    void init() {
        level.set_gravity(0.0f, -25.0f);
        level.simulationSteps = 10;
    }

    void add_rigid_body(ic::Physics::RigidObject2D *rigid) {
        level.add_object(rigid);
    }

    void update(ic::Entities &registry, float dt) {
        level.update(dt);

        /*
        for (auto &object : level.get_objects()) {
            if (!object->dynamic) continue;

            ic::Physics::RigidObject2D *rigid = dynamic_cast<ic::Physics::RigidObject2D*>(object);
            if (rigid == nullptr) continue;

            if (rigid->transform->position.y() < -50.0f) {
                rigid->transform->position.y() = -50.0f;
                rigid->velocity.y() *= -1.0f;
            }

            // Very far Y bound
            if (rigid->transform->position.y() > 2000.0f) {
                rigid->transform->position.y() = 2000.0f;
                rigid->velocity.y() *= -1.0f;
            }

            if (rigid->transform->position.x() < -50.0f) {
                rigid->transform->position.x() = -50.0f;
                rigid->velocity.x() *= -1.0f;
            }

            if (rigid->transform->position.x() > 50.0f) {
                rigid->transform->position.x() = 50.0f;
                rigid->velocity.x() *= -1.0f;
            }
        }
        */


        for (int i = 0; i < ic::lastEntityIndex; i++) {
            ic::Entity *entity = registry.get_entity(i);

            if (entity->has<TransformComp>() && entity->has<PhysicsComp>()) {
                ic::Vec2f pos = entity->get<PhysicsComp>().rigidBody->transform->position;
                entity->get<TransformComp>().position = pos;
            }
        }
    }
};

struct CameraSystem {
    void update(ic::Entities &registry, float dt) {
        for (int i = 0; i < ic::lastEntityIndex; i++) {
            ic::Entity *entity = registry.get_entity(i);

            if (entity->has<TransformComp>()) {
                auto &transform = entity->get<TransformComp>();

                if (entity->has<CameraFollowComp>()) { 
                    entity->get<CameraFollowComp>().camera->position = transform.position + ic::Vec2f({ 4.5f, 1.0f });
                }
            }
        }
    }
};

namespace Entities {
    SpriteSystem spriteSystem;
    PhysicsSystem physicsSystem;
    MovementSystem movementSystem;
    CameraSystem cameraSystem;

    ic::Entity *add_rectangle(ic::Entity *entity, const std::string &texturePath, float x, float y, float width, float height) {
        entity->add<SpriteComp>(
            ic::GeometryGenerator::get().generate_rectangle_mesh(width, height, width * 2.0f, height * 2.0f),
            ic::TextureLoader::get().load_png(texturePath)
        );

        entity->add<TransformComp>();
        physicsSystem.add_rigid_body(
            entity->add<PhysicsComp>(
                new ic::Physics::RectangleCollider(width, height),
                ic::Vec2f(x, y),
                1.0f, 2.0f, false
            ).rigidBody
        );

        return entity;
    }

    ic::Entity *add_obstacle_rectangle(ic::Entity *entity, const std::string &texturePath, float x, float y, float width, float height) {
        ic::Entity *rectangle = add_rectangle(entity, texturePath, x, y, width, height);

        rectangle->get<PhysicsComp>().rigidBody->collision([&](ic::Physics::Manifold2D manifold, float dt) {
            if (manifold.points.normal.x() > 0.0f) {
                manifold.object1->set_position(0.0f, 0.5f);
                dynamic_cast<ic::Physics::RigidObject2D*>(manifold.object1)->velocity = { 5.0f, 0.0f };
            }
        });

        return rectangle;
    }

    ic::Entity *add_obstacle(ic::Entity *entity, float x, float y) {
        //std::vector<float> positions = {
        //    -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f
        //};
//
        //ic::Mesh2D mesh;
        //mesh.add_attribute(0, 2, positions);
        //mesh.set_index_buffer({ 0, 1, 2 });
        //mesh.set_material(ic::MeshMaterial2D(ic::Colors::lightGray));

        entity->add<SpriteComp>(
            ic::GeometryGenerator::get().generate_rectangle_mesh(0.2f, 0.4f),
            ic::TextureLoader::get().load_png("resources/textures/white.png")
        );

        entity->add<TransformComp>();
        physicsSystem.add_rigid_body(
            entity->add<PhysicsComp>(
                new ic::Physics::RectangleCollider(0.2f, 0.4f),
                ic::Vec2f(x, y),
                1.0f, 2.0f, false
            ).rigidBody
        );

        entity->get<PhysicsComp>().rigidBody->collision([&](ic::Physics::Manifold2D manifold, float dt) {
            manifold.object1->set_position(0.0f, 0.5f);
            dynamic_cast<ic::Physics::RigidObject2D*>(manifold.object1)->velocity = { 5.0f, 0.0f };
        });

        return entity;
    }
};

class Platformer : public ic::Application {
    ic::Entities registry;


    ic::Shader spriteShader;
    ic::Camera2D camera;

    public:
        bool init() override {
            displayName = "Platformer";

            return true;
        }
        
        bool load() override {
            camera = ic::Camera2D();
            spriteShader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D);
            Entities::movementSystem.load();
            Entities::physicsSystem.init();
            Entities::spriteSystem.init();
            
            // A floor that has a wood texture
            Entities::add_rectangle(registry.add_entity(), "resources/textures/wood.png", 0.0f, -0.5f, 1000.0f, 0.5f);
            for (int i = 0; i < 30; i++) {
                float x = i * 5.0f + 9.0f;
                float y = 0.5f + rand() % 5;

                Entities::add_obstacle_rectangle(registry.add_entity(), "resources/textures/stone-bricks.png", x, y, 0.5f, 0.5f);
            }
            for (int i = 0; i < 60; i++) {
                float x = i * 2.5f + 9.0f;
                float y = 0.5f + rand() % 5;

                Entities::add_obstacle(registry.add_entity(), x + 10.0f, y);
            }

            // The player
            {
                ic::Entity *entity = registry.add_entity();
                entity->add<SpriteComp>(
                    ic::GeometryGenerator::get().generate_rectangle_mesh(0.5f, 0.5f),
                    ic::TextureLoader::get().load_png("resources/textures/white.png")
                ).mesh.set_material(ic::MeshMaterial2D(ic::Colors::green, 1.0f));

                entity->add<TransformComp>();
                Entities::physicsSystem.add_rigid_body(
                    entity->add<PhysicsComp>(
                        new ic::Physics::RectangleCollider(0.5f, 0.5f),
                        ic::Vec2(0.0f, 0.5f),
                        1.0f, 2.0f, true
                    ).rigidBody->apply_velocity(5.0f, 0.0f)
                );

                //entity->add<MovementComp>(10.5f, true);
                entity->add<JumpComp>(12.0f);
                entity->add<CameraFollowComp>(&Entities::spriteSystem.camera);
            }

            return true;
        }

        void window_size_changed(int w, int h) override {
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            Entities::movementSystem.handle_event(registry, event, dt);

            return true;
        }

        bool update(float dt) override {
            Entities::physicsSystem.update(registry, dt);
            Entities::movementSystem.update(registry, dt);

            Entities::cameraSystem.update(registry, dt);
            Entities::spriteSystem.update(registry, dt);

            clear_color(ic::Colors::blue);

            spriteShader.use();
            camera.use(spriteShader);
            Entities::spriteSystem.draw(registry, spriteShader);

            return true; 
        }

        void dispose() override {
            spriteShader.clear();
        }
};

int main(int argc, char *argv[]) {
    Platformer application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}