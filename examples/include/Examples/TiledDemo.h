#ifndef IC_EXAMPLE_TILED_DEMO_H
#define IC_EXAMPLE_TILED_DEMO_H

#include <Icosahedron/Core.h>

const std::size_t MAP_WIDTH = 16;
const std::size_t MAP_HEIGHT = 16;
const std::size_t MAP_AREA = MAP_WIDTH * MAP_HEIGHT;
const int DROP_DISPARITY = 5;
const float COMPLETE_AFTER_SECONDS = 5.0f;
const float COMPLETE_POPUP_ANIMATION_DURATION = 1.5f;

const std::array<int, MAP_AREA> tiles = {
    0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 1, 1, 0, 2, 2, 2, 0, 0, 0, 0,
    0, 2, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0,
    0, 1, 0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 1, 1, 2, 0, 1, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0,
};

const std::array<int, MAP_AREA> obstructing = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

struct RectangleShape {
    ic::Rectangle r;
    std::string atlasEntryName;

    RectangleShape(ic::Vec2f position, ic::Vec2f size, std::string atlasEntryName) : r(position, size), atlasEntryName(atlasEntryName) {

    }
};

struct PolygonShape {
    ic::Polygon poly;
    ic::Vec2f position;
    std::vector<unsigned int> indices;

    PolygonShape(std::vector<float> vertCoords, ic::Vec2f pos = { 0.0f, 0.0f }) : poly(vertCoords), position(pos) {
        poly.translate(position);
        indices = ic::EarClippingTriangulation::get().triangulate(vertCoords);
    }

    void draw(ic::Renderer &renderer, ic::Batch *batch, ic::Color color) {
        std::vector<ic::Vec2f> positions;
        auto components = poly.get_transformed_vertices();

        for (int i = 0; i < components.size(); i += 2) {
            float x = components[i], y = components[i + 1];
            ic::Vec2f position = { x, y };

            positions.push_back(position);
        }
        
        
        renderer.draw_vertices(batch, positions, indices, ic::Colors::lightGray);
    }
};

struct Drop {
    PolygonShape *shape;
    ic::Rectangle hitbox;

    Drop() {
        this->shape = new PolygonShape(ic::GeometryGenerator::get().generate_regular_polygon(3));
        this->hitbox = ic::Rectangle({ 0.0f, 0.0f }, { 0.8f, 0.8f });
    }

    Drop(ic::Vec2f position, int sides, float radius) {
        this->shape = new PolygonShape(ic::GeometryGenerator::get().generate_regular_polygon(sides, radius), position);
        this->hitbox = ic::Rectangle(position, { radius - radius * 0.2f, radius - radius * 0.2f });
    }
};

/* A tiled game. Collision detection is via AABBs (ic::Rectangles). */
class TiledDemo : public ic::Application {
    ic::Batch *batch, *textBatch;
    ic::TextureAtlas *texture;
    ic::Shader *shader, *textShader;
    ic::TextAtlas *atlas;

    RectangleShape *shape;
    std::vector<Drop*> drops;
    
    ic::Camera2D *camera, *uiCamera;

    bool collisionDebug;
    int collected;
    int initialSize;

    bool completedLevel;
    float completeTimer;

    public:
        bool init() override {
            displayName = "Example window";
            scaling = ic::WindowScaling::fullscreen;

            return true;
        }
        
        bool load() override {
            shader = ic::ShaderLoader::get().load(shaders.basicTextureShaderVertex2D, shaders.basicTextureShaderFrag2D);
            textShader = ic::ShaderLoader::get().load(shaders.basicTextShaderVertex2D, shaders.basicTextShaderFrag2D);

            camera = new ic::Camera2D(0.3f);
            uiCamera = new ic::Camera2D();

            batch = new ic::Batch(10000, ic::TRIANGLES);
            textBatch = new ic::Batch(1000, ic::TRIANGLES);
            texture = new ic::TextureAtlas();
            texture->add_entries({ "white", "resources/textures/white.png",
                                   "ball", "resources/textures/ball.png",
                                   "player", "resources/textures/white.png",
                                   "wood", "resources/textures/wood.png",
                                   "stone", "resources/textures/stone.png",
                                   "dirt", "resources/textures/dirt.png",
                                   "grass", "resources/textures/grass.png" });

            // We use the roboto font
            ic::FreeType::get().add_atlas("score", "resources/fonts/Roboto-Regular.ttf", 48);
            atlas = ic::FreeType::get().find_atlas("score");

            
            ic::KeyboardController *debugCont = new ic::KeyboardController();
            debugCont->add_key_up_action([this]() {collisionDebug = !collisionDebug;}, KEY_T);
            
            inputHandler.add_input((new ic::KeyboardController())->with_WASD(), "WASD");
            inputHandler.add_input(debugCont, "collisionDebug");


            shape = new RectangleShape({ 0.0f, 0.0f }, { 0.4f, 0.4f }, "player");
            
            collisionDebug = false;
            reset();
            
            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override {
            // Collision detection
            std::vector<ic::Rectangle> possibleCollisions;

            int px = (int) shape->r.position.x();
            int py = (int) shape->r.position.y();

            float detectionRadius1 = shape->r.size.x() + 1;
            float detectionRadius2 = shape->r.size.y() + 1;

            for (int i = -detectionRadius1; i < detectionRadius1 + 1; i++) {
                for (int j = -detectionRadius2; j < detectionRadius2 + 1; j++) {
                    int cx = px + i;
                    int cy = py + j;

                    // Boundary conditions
                    if ((cx < 0 || cy < 0) || (cx >= MAP_WIDTH || cy >= MAP_HEIGHT)) continue;

                    if (obstructing[cy * MAP_WIDTH + cx]) possibleCollisions.push_back({ {(float)cx, (float)cy}, {0.5f, 0.5f} });
                }
            }

            for (auto rectangle : possibleCollisions) {
                if (!shape->r.overlaps(rectangle)) continue;

                ic::Vec2f overlap = shape->r.find_overlap(rectangle);

                if (overlap.x() < overlap.y()) {
                    int sign = (shape->r.position.x() >= rectangle.position.x()) ? 1 : -1;
                    shape->r.position.x() += sign * overlap.x();
                } else {
                    int sign = (shape->r.position.y() >= rectangle.position.y()) ? 1 : -1;
                    shape->r.position.y() += sign * overlap.y();
                }
            }

            // Remove drops if colliding
            for (auto &drop : drops) {
                if (!drop->hitbox.overlaps(shape->r)) continue;
                collect(drop);
            }

            // If level is completed...
            if (completedLevel) {
                completeTimer += dt;
            }
            if (completeTimer >= COMPLETE_AFTER_SECONDS) {
                reset();
            }

            // Dynamics
            float speed = 3.0f;
            auto *controller = inputHandler.find_keyboard("WASD");
            ic::Vec2i dir = controller->direction;

            shape->r.position.x() += dir.x() * speed * dt;
            shape->r.position.y() += dir.y() * speed * dt;

            shape->r.position.clamp({ -0.5f + shape->r.size.x(), -0.5f + shape->r.size.y() }, { MAP_WIDTH - shape->r.size.x() - 0.5f, MAP_HEIGHT - shape->r.size.y() - 0.5f });

            camera->position = shape->r.position;
            for (auto &drop : drops) {
                drop->shape->poly.rotate(dt);
            }

            // Rendering
            clear_color(ic::Colors::blue);

            shader->use();
            camera->use(shader);

            texture->use();

            for (int i = 0; i < MAP_AREA; i++) {
                int index = tiles[i];
                std::string entryName;

                switch (index) {
                    case 0: entryName = "grass"; break;
                    case 1: entryName = "dirt"; break;
                    case 2: entryName = "stone"; break;
                }


                int x = i % MAP_WIDTH;
                int y = i / MAP_HEIGHT;

                renderer.draw_rectangle(batch, texture->get_entry(entryName), x, y, 0.5f, 0.5f);
            }

            if (collisionDebug) {
                for (int i = -detectionRadius1; i < detectionRadius1 + 1; i++) {
                    for (int j = -detectionRadius2; j < detectionRadius2 + 1; j++) {
                        int cx = px + i;
                        int cy = py + j;

                        // Boundary conditions
                        if ((cx < 0 || cy < 0) || (cx >= MAP_WIDTH || cy >= MAP_HEIGHT)) continue;


                        std::string sprite = "ball";
                        if (obstructing[cy * MAP_WIDTH + cx]) sprite = "white";

                        renderer.draw_rectangle(batch, texture->get_entry(sprite), cx, cy, 0.4f, 0.4f);
                    }
                }
            }

            renderer.draw_rectangle(batch, texture->get_entry(shape->atlasEntryName),
                shape->r.position.x(), shape->r.position.y(), 
                shape->r.size.x(),     shape->r.size.y(), 
                ic::Colors::green);

            for (auto &drop : drops) {
                drop->shape->draw(renderer, batch, ic::Colors::lightGray);
            }

            batch->render();

            // Text rendering
            textShader->use();
            uiCamera->use(textShader);

            atlas->use();
            renderer.draw_string(textBatch, atlas, "T key - toggle potential collisions", -1.0f, 0.9f, 0.8f, 0.8f);
            renderer.draw_string(textBatch, atlas, std::to_string(collected) + "/" + std::to_string(initialSize) + " polygons remaining", -1.0f, 0.75f, 0.8f, 0.8f);

            if (completedLevel) {
                float t = completeTimer / COMPLETE_POPUP_ANIMATION_DURATION;
                float alpha = ic::Mathf::get().clamp(t, 0.0f, 1.0f);

                ic::Vec2f from = { 0.0f, 1.1f }, to = { 0.0f, 0.4f };
                ic::Vec2f position = from.interpolate(to, ic::Interpolation::get().smoothstep(alpha));
                float scale = ic::Mathf::get().interpolate_logarithmic(0.5f, 1.5f, alpha);

                renderer.draw_string_centered(textBatch, atlas, "Level finished!", position.x(), position.y(), scale, scale);
            }
            textBatch->render();

            return true; 
        }

        void dispose() override {
            batch->dispose();
            textBatch->dispose();
            texture->dispose();
            shader->clear();
            textShader->clear();
        }

        void collect(Drop *drop) {
            int index = 0;

            for (int i = 0; i < drops.size(); i++) {
                if (drop == drops[i]) {
                    index = i;
                    break;
                }
            }
            drops.erase(drops.begin() + index);
            delete drop;

            collected++;
            if (collected == initialSize) {
                completedLevel = true;
            }
        }

        void reset() {
            for (int x = 0; x < MAP_WIDTH; x++) {
                for (int y = 0; y < MAP_HEIGHT; y++) {
                    int index = y * MAP_WIDTH + x;
                    if (obstructing[index]) continue;
                    
                    if (rand() % DROP_DISPARITY == 1) {
                        Drop *drop = new Drop({ 0.0f + x, 0.0f + y }, rand() % 5 + 3, (rand() % 50 / 300.0f) + 0.15f);
                        drop->shape->poly.set_rotation(ic::Mathf::get().to_radians(rand() % 360));

                        drops.push_back(drop);
                    }
                }
            }
            
            shape->r.position = { 0.0f, 0.0f };

            collected = 0;
            initialSize = drops.size();
            completedLevel = false;
            completeTimer = 0.0f;
        }
};

#endif