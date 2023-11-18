#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Texture.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>

#include <unordered_map>



using entity_t = std::size_t;
entity_t lastEntityIndex = 0;

std::size_t add_entity() {
    static entity_t entities = 0;

    entities++;
    lastEntityIndex = entities;

    return entities;
}


struct PositionComp {
    ic::Vec2f position;
    ic::Vec2f scaling;
};

struct SpriteComp {
    ic::Mesh2D mesh;
    ic::Texture texture;
};

struct MovementComp {};


struct ComponentRegistry {
    std::unordered_map<entity_t, PositionComp> positions;
    std::unordered_map<entity_t, SpriteComp> sprites;
    std::unordered_map<entity_t, MovementComp> movements;

    template <typename T>
    bool has_component(const entity_t &entity, const std::unordered_map<entity_t, T> &components) {
        return (components.find(entity) != components.end());
    }
};

struct SpriteSystem {
    void update(ComponentRegistry &registry, float dt) {
        for (int i = 1; i <= lastEntityIndex; i++) {
            if (registry.has_component<SpriteComp>(i, registry.sprites) && registry.has_component<PositionComp>(i, registry.positions)) {
                // Matrices
                ic::Mat4x4 translation = ic::Mat4x4().set_translation<2>(registry.positions[i].position);
                ic::Mat4x4 scaling = ic::Mat4x4().set_scaling<2>(registry.positions[i].scaling);

                registry.sprites[i].mesh.set_transformation(translation * scaling);
            }
        }
    }

    void draw(ComponentRegistry &registry, ic::Shader &shader) {
        for (int i = 1; i <= lastEntityIndex; i++) {
            if (registry.has_component<SpriteComp>(i, registry.sprites)) {
                registry.sprites[i].texture.use();
                registry.sprites[i].mesh.draw(shader);
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

    void update(ComponentRegistry &registry, float dt) {
        auto *controller = ic::InputHandler::get().find_keyboard("WASD");
        ic::Vec2i dir = controller->get_direction();
        float speed = 1.0f;
        
        for (int i = 1; i <= lastEntityIndex; i++) {
            if (registry.has_component<PositionComp>(i, registry.positions) && registry.has_component<MovementComp>(i, registry.movements)) {
                // Currently move all entities with MovementComp in the direction of the controller
                registry.positions[i].position.x() += dir.x() * speed * dt;
                registry.positions[i].position.y() += dir.y() * speed * dt;
            }
        }
    }
};



class EntityComponentSystem : public ic::Application {
    ComponentRegistry registry;
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
                entity_t entity = add_entity();
                registry.sprites[entity] = SpriteComp {
                    ic::GeometryGenerator::get().generate_rectangle_mesh(1.0f, 1.0f, 3.0f, 3.0f),
                    ic::TextureLoader::get().load_png("resources/textures/wood.png"),
                };
            }

            // Stone floor
            {
                entity_t entity = add_entity();
                registry.sprites[entity] = SpriteComp {
                    ic::GeometryGenerator::get().generate_rectangle_mesh(1.0f, 1.0f, 0.4f, 0.6f),
                    ic::TextureLoader::get().load_png("resources/textures/stone-bricks.png"),
                };
    
                registry.positions[entity] = PositionComp {
                    { 0.5f, 0.5f },
                    { 0.3f, 0.2f },
                };
            }

            // The player
            {
                entity_t entity = add_entity();
                registry.sprites[entity] = SpriteComp {
                    ic::GeometryGenerator::get().generate_rectangle_mesh(1.0f, 1.0f),
                    ic::TextureLoader::get().load_png("resources/textures/white.png"),
                };
                registry.sprites[entity].mesh.set_material(ic::MeshMaterial2D(ic::Colors::green, 1.0f));
    
                registry.positions[entity] = PositionComp {
                    { -0.8f, 0.5f },
                    { 0.2f, 0.2f },
                };
    
                registry.movements[entity] = MovementComp {};
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