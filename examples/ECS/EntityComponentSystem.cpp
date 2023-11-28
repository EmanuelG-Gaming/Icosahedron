#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Texture.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>

#include <Icosahedron/entity/Entity.h>
#include <Icosahedron/entity/Entities.h>



struct PositionComp : public ic::Component {
    ic::Vec2f position;
    ic::Vec2f scaling;

    PositionComp() {}
    PositionComp(float x, float y, float width, float height) : position({ x, y }), scaling({ width, height }) {}
};

struct SpriteComp : public ic::Component {
    ic::Mesh2D mesh;
    ic::Texture texture;

    SpriteComp() {}
    SpriteComp(ic::Mesh2D mesh, ic::Texture texture) : mesh(mesh), texture(texture) {} 
};

struct MovementComp : public ic::Component {
    float speed = 1.0f;

    MovementComp() {}
    MovementComp(float speed) : speed(speed) {}
};


struct SpriteSystem {
    void update(ic::Entities &registry, float dt) {
        for (int i = 0; i < ic::lastEntityIndex; i++) {
            ic::Entity *entity = registry.get_entity(i);

            if (entity->has<SpriteComp>() && entity->has<PositionComp>()) {
                auto &transform = entity->get<PositionComp>();

                // Matrices
                ic::Mat4x4 translation = ic::Mat4x4().set_translation<2>(transform.position);
                ic::Mat4x4 scaling = ic::Mat4x4().set_scaling<2>(transform.scaling);

                entity->get<SpriteComp>().mesh.set_transformation(translation * scaling);
            }
        }
    }

    void draw(ic::Entities &registry, ic::Shader &shader) {
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

    void update(ic::Entities &registry, float dt) {
        ic::Vec2i dir = keyboard->get_direction();
        
        for (int i = 0; i < ic::lastEntityIndex; i++) {
            ic::Entity *entity = registry.get_entity(i);

            if (entity->has<PositionComp>() && entity->has<MovementComp>()) {
                auto &transform = entity->get<PositionComp>();
                float deltaSpeed = entity->get<MovementComp>().speed * dt;

                // Currently move all entities with MovementComp in the direction of the controller
                transform.position.x() += dir.x() * deltaSpeed;
                transform.position.y() += dir.y() * deltaSpeed;
            }
        }
    }
};


class EntityComponentSystem : public ic::Application {
    ic::Entities registry;
    SpriteSystem spriteSystem;
    MovementSystem movementSystem;

    ic::Shader spriteShader;
    ic::Camera2D camera;

    public:
        bool init() override {
            displayName = "Entity component system";

            return true;
        }
        
        bool load() override {
            camera = ic::Camera2D();
            spriteShader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D);
            movementSystem.load();

            // A floor that has a wood texture
            {
                ic::Entity *entity = registry.add_entity();
                entity->add<SpriteComp>(
                    ic::GeometryGenerator::get().generate_rectangle_mesh(1.0f, 1.0f, 3.0f, 3.0f),
                    ic::TextureLoader::get().load_png("resources/textures/wood.png")
                );
            }

            // Stone floor
            {
                ic::Entity *entity = registry.add_entity();
                entity->add<SpriteComp>(
                    ic::GeometryGenerator::get().generate_rectangle_mesh(1.0f, 1.0f, 0.4f, 0.6f),
                    ic::TextureLoader::get().load_png("resources/textures/stone-bricks.png")
                );
                entity->add<PositionComp>(
                    0.5f, 0.5f,
                    0.3f, 0.2f
                );
            }

            // The player
            {
                ic::Entity *entity = registry.add_entity();
                entity->add<SpriteComp>(
                    ic::GeometryGenerator::get().generate_rectangle_mesh(1.0f, 1.0f),
                    ic::TextureLoader::get().load_png("resources/textures/white.png")
                ).mesh.set_material(ic::MeshMaterial2D(ic::Colors::green, 1.0f));

                entity->add<PositionComp>(
                    -0.8f, 0.5f,
                    0.2f, 0.2f
                );
                entity->add<MovementComp>(1.5f);
            }

            return true;
        }

        void window_size_changed(int w, int h) override {
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            movementSystem.update(registry, dt);
            spriteSystem.update(registry, dt);

            clear_color(ic::Colors::blue);

            spriteShader.use();
            camera.use(spriteShader);
            spriteSystem.draw(registry, spriteShader);

            return true; 
        }

        void dispose() override {
            spriteShader.clear();
        }
};

int main(int argc, char *argv[]) {
    EntityComponentSystem application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}