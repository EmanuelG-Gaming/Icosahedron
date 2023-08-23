// Examples (uncomment any of these)
/* Example 1: Displays a blue window on startup. */
/*
#include <Icosahedron/Core.h>

class Example1 : public ic::Application {
    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }
        
        bool load() override {
            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override { 
            clear_color(ic::Colors::blue);
            return true; 
        }
};

int main(int argc, char *argv[]) {
    Example1 application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/


/* Example2: Displays a textured rectangle that can be moved around using the WASD keys or the arrow keys. */
/*
#include <Icosahedron/Core.h>

class Example2 : public ic::Application {
    ic::Texture<ic::T2D> *texture;
    ic::Shader *shader;

    ic::Mesh2D *shape;
    ic::Vec2f shapePosition;

    public:
        bool init() override {
            displayName = "Example window";
            
            return true;
        }
        
        bool load() override {
            shape = new ic::Mesh2D(ic::GeometryGenerator::get().generate_rectangle(0.2f, 0.2f));

            shape->jump_attribute();
            shape->add_attribute("textureCoords", 2, { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f });
            shape->set_index_buffer({ 0, 1, 2, 0, 2, 3 });
            shape->set_material(ic::MeshMaterial2D(ic::Colors::white, 1.0f));

            texture = new ic::Texture<ic::T2D>({"resources/textures/wood.png"});
            shader = new ic::Shader(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D, false);

            inputHandler.add_input((new ic::KeyboardController())->with_WASD(), "WASD");
            shapePosition = { 0.0f, 0.0f };

            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override { 
            float speed = 1.0f;
            ic::KeyboardController *controller = (ic::KeyboardController*) inputHandler.find_input("WASD");
            ic::Vec2i dir = controller->direction;

            shapePosition.x() += dir.x() * speed * dt;
            shapePosition.y() += dir.y() * speed * dt;
            
            shape->set_transformation(ic::Mat4x4().set_translation<2>(shapePosition));

            clear_color(ic::Colors::blue);

            shader->use();
            texture->use();
            shape->draw(shader);
            
            return true; 
        }

        void dispose() override {
            texture->dispose();
            shader->clear();

            shape->dispose();
        }
};

int main(int argc, char *argv[]) {
    Example2 application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/

/* Example3: A recreation of pong. The left paddle uses WS keys and the right one uses up-down keys. */
/*
#include <Icosahedron/Core.h>

class Example3 : public ic::Application {
    ic::Batch2D *batch, *textBatch;
    ic::TextureAtlas *texture;
    ic::TextAtlas *atlas;
    ic::Shader *shader, *textShader;

    ic::RectangleShape *paddle1, *paddle2, *ball;
    ic::Vec2f vel;
    int points1 = 0, points2 = 0;

    public:
        bool init() override {
            displayName = "Example window";
            //scaling = ic::WindowScaling::fullscreen;

            return true;
        }
        
        bool load() override {
            shader = new ic::Shader(shaders.basicTextureShaderVertex2D, shaders.basicTextureShaderFrag2D, false);
            textShader = new ic::Shader(shaders.basicTextShaderVertex2D, shaders.basicTextShaderFrag2D, false);

            // We use the arial font
            ic::FreeType::get().add_atlas("score", "C:/Windows/Fonts/arial.ttf", 48);
            atlas = ic::FreeType::get().find_atlas("score");

            texture = new ic::TextureAtlas();
            texture->add_entries({ "paddle1", "resources/textures/white.png",
                                   "paddle2", "resources/textures/white.png",
                                   "ball", "resources/textures/ball.png" });

            batch = new ic::Batch2D(1000, ic::TRIANGLES);
            textBatch = new ic::Batch2D(1000, ic::TRIANGLES);
            
            paddle1 = new ic::RectangleShape({ -0.7f, 0.0f }, { 0.05f, 0.2f }, "paddle1");
            paddle2 = new ic::RectangleShape({ 0.7f, 0.0f }, { 0.05f, 0.2f }, "paddle2");
            ball = new ic::RectangleShape({ 0.0f, 0.0f }, { 0.05f, 0.05f }, "ball");

            paddle1->set_atlas(texture);
            paddle2->set_atlas(texture);
            ball->set_atlas(texture);

            restart(nullptr);

            ic::KeyboardController *cont1 = new ic::KeyboardController();
            cont1->add_action([cont1]() {cont1->direction.y() = 1;}, KEY_W);
            cont1->add_action([cont1]() {cont1->direction.y() = -1;}, KEY_S);

            
            ic::KeyboardController *cont2 = new ic::KeyboardController();
            cont2->add_action([cont2]() {cont2->direction.y() = 1;}, KEY_UP);
            cont2->add_action([cont2]() {cont2->direction.y() = -1;}, KEY_DOWN);
            

            inputHandler.add_input(cont1, "paddle1");
            inputHandler.add_input(cont2, "paddle2");

            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override {
            // Boundary conditions
            if (ball->r.position.y() + ball->r.size.y() > 1.0f) {
                ball->r.position.y() = 1.0f - ball->r.size.y();
                vel.y() *= -1;
            }
            if (ball->r.position.y() - ball->r.size.y() < -1.0f) {
                ball->r.position.y() = -1.0f + ball->r.size.y();
                vel.y() *= -1;
            }

            if (ball->r.position.x() + ball->r.size.x() < -1.0f) {
                restart(paddle2);
            }
            if (ball->r.position.x() - ball->r.size.x() > 1.0f) {
                restart(paddle1);
            }

            // Collision detection
            if (ball->r.overlaps(paddle1->r)) {
                ic::Vec2f overlap = ball->r.find_overlap(paddle1->r);
                if (overlap.x() < overlap.y()) {
                    ball->r.position.x() += (overlap.x() + ball->r.size.x());
                    vel.x() *= -1;
                } else {
                    int sign = (ball->r.position.y() >= paddle1->r.position.y()) ? 1 : -1;
                    ball->r.position.y() += sign * (overlap.y() + ball->r.size.y());
                    vel.y() *= -1;
                }
            }

            if (ball->r.overlaps(paddle2->r)) {
                ic::Vec2f overlap = ball->r.find_overlap(paddle2->r);
                if (overlap.x() < overlap.y()) {
                    ball->r.position.x() -= (overlap.x() + ball->r.size.x());
                    vel.x() *= -1;
                } else {
                    int sign = (ball->r.position.y() >= paddle2->r.position.y()) ? 1 : -1;
                    ball->r.position.y() += sign * (overlap.y() + ball->r.size.y());
                    vel.y() *= -1;
                }
            }

            // Dynamics
            ball->r.position.x() += vel.x() * dt;
            ball->r.position.y() += vel.y() * dt;



            float speed = 1.0f;
            ic::KeyboardController *controller1 = (ic::KeyboardController*) inputHandler.find_input("paddle1");
            ic::KeyboardController *controller2 = (ic::KeyboardController*) inputHandler.find_input("paddle2");
            
            ic::Vec2i dir1 = controller1->direction;
            ic::Vec2i dir2 = controller2->direction;

            
            paddle1->r.position.y() += dir1.y() * speed * dt;
            paddle2->r.position.y() += dir2.y() * speed * dt;

            paddle1->r.position.clamp({ -1.0f, -1.0f + paddle1->r.size.y() }, { 1.0f, 1.0f - paddle1->r.size.y() });
            paddle2->r.position.clamp({ -1.0f, -1.0f + paddle2->r.size.y() }, { 1.0f, 1.0f - paddle2->r.size.y() });
            
            // Rendering
            clear_color(ic::Colors::black);

            shader->use();
            texture->use();
            paddle1->draw(renderer, batch, ic::Colors::white);
            paddle2->draw(renderer, batch, ic::Colors::white);
            ball->draw(renderer, batch, ic::Colors::white);
            batch->render();

            // Text rendering
            textShader->use();
            atlas->use();
            renderer.draw_string_centered(textBatch, atlas, std::to_string(points1) + " | " + std::to_string(points2), 0.0f, 0.7f);
            textBatch->render();

            return true; 
        }

        void dispose() override {
            batch->dispose();
            textBatch->dispose();
            texture->dispose();
            shader->clear();
        }

        void restart(ic::RectangleShape *winner) {
            if (winner == paddle1) {
                points1++;
            } else if (winner == paddle2) {
                points2++;
            }

            paddle1->r.position = { -0.7f, 0.0f };
            paddle2->r.position = { 0.7f, 0.0f };

            ball->r.position = { 0.0f, 0.0f };
            vel = { 1.0f, 0.5f };
        }
};

int main(int argc, char *argv[]) {
    Example3 application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/

/* Example4: A tiled game. Collision detection is via AABBs (ic::Rectangles). */
/*
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

struct Drop {
    ic::PolygonShape *shape;
    ic::Rectangle hitbox;

    Drop() {
        this->shape = new ic::PolygonShape(ic::GeometryGenerator::get().generate_regular_polygon(3));
        this->hitbox = ic::Rectangle({ 0.0f, 0.0f }, { 0.8f, 0.8f });
    }

    Drop(ic::Vec2f position, int sides, float radius) {
        this->shape = new ic::PolygonShape(ic::GeometryGenerator::get().generate_regular_polygon(sides, radius), position);
        this->hitbox = ic::Rectangle(position, { radius - radius * 0.2f, radius - radius * 0.2f });
    }
};

class Example4 : public ic::Application {
    ic::Batch2D *batch, *textBatch;
    ic::TextureAtlas *texture;
    ic::Shader *shader, *textShader;
    ic::TextAtlas *atlas;

    ic::RectangleShape *shape;
    std::vector<Drop*> drops;
    
    ic::Camera2D *camera;

    bool collisionDebug;
    int collected;
    int initialSize;

    bool completedLevel;
    float completeTimer;

    public:
        bool init() override {
            displayName = "Example window";
            //scaling = ic::WindowScaling::fullscreen;

            return true;
        }
        
        bool load() override {
            shader = new ic::Shader(shaders.basicTextureShaderVertex2D, shaders.basicTextureShaderFrag2D, false);
            textShader = new ic::Shader(shaders.basicTextShaderVertex2D, shaders.basicTextShaderFrag2D, false);

            camera = new ic::Camera2D(0.3f);

            batch = new ic::Batch2D(10000, ic::TRIANGLES);
            textBatch = new ic::Batch2D(1000, ic::TRIANGLES);
            texture = new ic::TextureAtlas();
            texture->add_entries({ "white", "resources/textures/white.png",
                                   "ball", "resources/textures/ball.png",
                                   "player", "resources/textures/white.png",
                                   "wood", "resources/textures/wood.png",
                                   "stone", "resources/textures/stone.png",
                                   "dirt", "resources/textures/dirt.png",
                                   "grass", "resources/textures/grass.png" });

            // We use the arial font
            ic::FreeType::get().add_atlas("score", "C:/Windows/Fonts/arial.ttf", 48);
            atlas = ic::FreeType::get().find_atlas("score");

            
            ic::KeyboardController *debugCont = new ic::KeyboardController();
            debugCont->add_key_up_action([this]() {collisionDebug = !collisionDebug;}, KEY_T);
            
            inputHandler.add_input((new ic::KeyboardController())->with_WASD(), "WASD");
            inputHandler.add_input(debugCont, "collisionDebug");


            shape = new ic::RectangleShape({ 0.0f, 0.0f }, { 0.4f, 0.4f }, "player");
            shape->set_atlas(texture);
            
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
            ic::KeyboardController *controller = (ic::KeyboardController*) inputHandler.find_input("WASD");
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

            shape->draw(renderer, batch, ic::Colors::green);
            for (auto &drop : drops) {
                drop->shape->draw(renderer, batch, ic::Colors::lightGray);
            }

            batch->render();

            // Text rendering
            textShader->use();
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

int main(int argc, char *argv[]) {
    Example4 application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/

/* Example5: Polygon example. Demonstrates the use of per-vertex colors, separate texturing (no atlas), matrix transformations, all in the same shader program. */
/*
#include <Icosahedron/Core.h>

class Example5 : public ic::Application {
    ic::Mesh2D *mesh1;
    ic::Mesh2D *mesh2;
    ic::Texture<ic::T2D> *texture, *whiteTexture;
    
    ic::Shader *shader;
    float time = 0;

    public:
        bool init() override {
            displayName = "Example window";
            
            return true;
        }
        
        bool load() override {
            // Mesh 1
            std::vector<float> positions = ic::GeometryGenerator::get().generate_regular_polygon(7, 0.3f);
            std::vector<float> textureCoords = ic::GeometryGenerator::get().generate_UV_polygon(positions);
            std::vector<unsigned int> indices = ic::EarClippingTriangulation::get().triangulate(positions);
            
            mesh1 = new ic::Mesh2D(positions);
            // Jump past the color attribute
            mesh1->jump_attribute();
            mesh1->add_attribute("textureCoords", 2, textureCoords);
            mesh1->set_index_buffer(indices);
            mesh1->set_material(ic::MeshMaterial2D(ic::Colors::white, 1.0f));
            
            // Mesh 2
            std::vector<float> triangle = ic::GeometryGenerator::get().generate_regular_polygon(3, 0.3f);
            
            mesh2 = new ic::Mesh2D(triangle);
            mesh2->add_attribute("color", 3, { ic::Colors::red, ic::Colors::green, ic::Colors::blue });
            mesh2->set_index_buffer({ 0, 1, 2 });
            mesh2->set_material(ic::MeshMaterial2D(ic::Colors::white, 0.2f));
            mesh2->set_transformation(ic::Mat4x4().set_translation<2>({ -0.35f, 0.0f }));


            shader = new ic::Shader(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D, false);
            
            texture = new ic::Texture<ic::T2D>({"resources/textures/wood.png"});
            whiteTexture = new ic::Texture<ic::T2D>({"resources/textures/white.png"});

            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            time += dt;
            clear_color(ic::Colors::blue);
            
            ic::Mat4x4 combined, scaling, rotation, translation;
            scaling.set_scaling<2>({ ic::Mathf::get().sinf(time), ic::Mathf::get().cosf(time) });
            rotation.set_rotation(time);
            translation.set_translation<2>({ 0.35f, 0.0f });
            
            // Multiplying matrices together this way follows column-major notation, so
            // translation * rotation * scaling is not the same as scaling * rotation * translation,
            // even though the latter seems to be a logical approach (i.e. you scale, then rotate, then translate)
            combined = translation * rotation * scaling;
            mesh1->set_transformation(combined);
            
            shader->use();
            texture->use();
            mesh1->draw(shader);
            whiteTexture->use();
            mesh2->draw(shader);

            return true; 
        }

        void dispose() {
            shader->clear();
            
            texture->dispose();
            whiteTexture->dispose();

            mesh1->dispose();
            mesh2->dispose();
        }
};

int main(int argc, char *argv[]) {
    Example5 application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/

/** Example6: */
/*
#include <Icosahedron/Core.h>

class Example6 : public ic::Application {
    ic::Mesh2D *woodMesh, *opacityMesh;
    ic::Texture<ic::T2D> *woodTexture, *opacityTexture;
    ic::Shader *shader;
    ic::Camera2D *camera;

    float rotation;

    public:
        bool init() override {
            displayName = "Example window";
            scaling = ic::WindowScaling::fullscreen;

            return true;
        }
        
        bool load() override {
            // Can you see what happens when you disable the blending?
            states.enable_blending(ic::SRC_ALPHA, ic::DEST_ONE_MINUS_SRC_ALPHA);
            
            auto positions = ic::GeometryGenerator::get().generate_rectangle(100.0f, 100.0f);
            auto textureCoords = ic::GeometryGenerator::get().generate_UV_rectangle(100.0f, 100.0f);

            woodMesh = new ic::Mesh2D(positions);
            woodMesh->jump_attribute();
            woodMesh->add_attribute("textureCoords", 2, textureCoords);
            woodMesh->set_index_buffer({ 0, 1, 2, 0, 2, 3 });
            woodMesh->set_material(ic::MeshMaterial2D(ic::Colors::white, 1.0f));

            opacityMesh = new ic::Mesh2D(positions);
            opacityMesh->jump_attribute();
            opacityMesh->add_attribute("textureCoords", 2, textureCoords);
            opacityMesh->set_index_buffer({ 0, 1, 2, 0, 2, 3 });
            opacityMesh->set_material(ic::MeshMaterial2D(ic::Colors::white, 1.0f));

            shader = new ic::Shader(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D, false);
            woodTexture = new ic::Texture<ic::T2D>({"resources/textures/wood.png"});
            opacityTexture = new ic::Texture<ic::T2D>({"resources/textures/side-alpha.png"});

            ic::MouseController *mouse = new ic::MouseController();
            mouse->add_mouse_up_action([this](){ rotation += ic::Mathf::get().pi * 2 / 5.0f; });

            inputHandler.add_input((new ic::KeyboardController())->with_WASD(), "WASD");
            inputHandler.add_input(mouse, "mouse");

            camera = new ic::Camera2D(0.5f);
            rotation = 0.0f;

            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override { 
            //rotation += dt;

            float speed = 3.0f;
            auto controller = inputHandler.find_keyboard("WASD");
            ic::Vec2i dir = controller->get_direction();

            camera->position.x() += dir.x() * speed * dt;
            camera->position.y() += dir.y() * speed * dt;
            

            clear_color(ic::Colors::blue);

            
            woodMesh->set_transformation(ic::Mat4x4().set_rotation(rotation));
            opacityMesh->set_transformation(ic::Mat4x4().set_rotation(rotation));

            shader->use();
            camera->use(shader);
            
            woodTexture->use();
            woodMesh->draw(shader);
            opacityTexture->use();
            opacityMesh->draw(shader);

            return true; 
        }

        void dispose() override {
            shader->clear();

            woodMesh->dispose();
            woodTexture->dispose();

            opacityMesh->dispose();
            opacityTexture->dispose();
        }
};

int main(int argc, char *argv[]) {
    Example6 application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/

#include <Icosahedron/Core.h>

class Example7 : public ic::Application {
    ic::Shader *blinnPhongShader;
    ic::Camera3D *camera;
    ic::Mesh3D *mesh;
    ic::Texture<ic::T2D> *woodTexture;

    float time;
    float rotationX, rotationY;
    public:
        bool init() override {
            displayName = "Example window";
            scaling = ic::WindowScaling::resizeable;

            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            states.enable_face_culling(ic::FRONT, ic::CCW);

            mesh = new ic::Mesh3D(ic::GeometryGenerator::get().generate_cube(0.5f));
            mesh->jump_attribute();
            mesh->add_attribute("textureCoords", 2, ic::GeometryGenerator::get().generate_UV_parallelipiped());
            mesh->add_attribute("normal", 3, ic::GeometryGenerator::get().generate_normal_parallelipiped());
            mesh->set_index_buffer(ic::GeometryGenerator::get().generate_parallelipiped_indices());
            
            std::string blinnPhongFragment = IC_ADD_GLSL_DEFINITION(
                precision mediump float;
                
                in vec3 vPosition;
                in vec2 vTCoords;
                in vec3 vNormal;

                struct PointLight {
                    vec3 position;

                    float constant;
                    float linear;
                    float quadratic;
                };

                const PointLight l = PointLight(vec3(0.0, 2.0, 1.0), 1.0, 0.09, 0.032);
                
                uniform sampler2D sampleTexture;
                uniform vec3 viewPosition;

                out vec4 outColor;

                vec4 compute_lighting(PointLight light) {
                    float distance = length(light.position - vPosition);
                    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));


                    vec3 normal = normalize(vNormal);
                    vec3 lightDirection = normalize(light.position - vPosition);
                    vec3 viewDirection = normalize(vPosition);
                    float dotProduct = dot(lightDirection, normal);

                    // Ambient reflection (indirect illumination approximation)
                    float ambientIntensity = 0.05 * attenuation;
                    vec4 ambientColor = vec4(ambientIntensity, ambientIntensity, ambientIntensity, 1.0);

                    // Diffuse reflection
                    float diffuseIntensity = clamp(dotProduct, 0.0, 1.0) * attenuation;
                    vec4 diffuseColor = texture(sampleTexture, vTCoords);
                    if (diffuseColor.a <= 0.1) diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
                    diffuseColor *= diffuseIntensity;

                    // Specular reflection
                    vec3 reflectDirection = normalize(lightDirection + viewDirection); // Blinn-Phong reflection
                    //vec3 reflectDirection = reflect(-lightDirection, normal); // Basic Phong reflection
                    float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.2 * 128.0) / 4.0) * attenuation;
                    vec4 specularColor = vec4(specularIntensity, specularIntensity, specularIntensity, 1.0);

                    vec4 result = ambientColor + diffuseColor + specularColor;

                    return result;
                }

                void main() {
                    vec4 color = compute_lighting(l);
                    outColor = color;
                }
            );

            blinnPhongShader = new ic::Shader(shaders.meshShaderVertex3D, blinnPhongFragment, false);
            woodTexture = new ic::Texture<ic::T2D>({"resources/textures/wood.png"});
            

            camera = new ic::Camera3D();
            camera->position = { -3.0f, 1.0f, 0.0f };
            camera->lookingAt = { 0.0f, 1.0f, 3.0f };

            inputHandler.add_input((new ic::KeyboardController())->with_WASD(), "WASD");
            inputHandler.add_input(new ic::MouseController(), "mouse");

            time = 0.0f;
            rotationX = rotationY = 0.0f;

            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            time += dt;

            float speed = 3.0f, sensitivity = 0.01f;
            auto controller = inputHandler.find_keyboard("WASD");
            auto mouse = inputHandler.find_mouse("mouse");

            ic::Vec2i dir = controller->get_direction();
            ic::Vec2i offset = mouse->get_relative_motion();

            camera->position.x() += dir.x() * speed * dt;
            camera->position.z() += dir.y() * speed * dt;
            rotationX -= offset.x() * sensitivity;
            rotationY -= offset.y() * sensitivity;

            if (rotationX > 2 * M_PI) rotationX = 0;
            if (rotationX < 0) rotationX = 2 * M_PI;
               
            if (rotationY > (89.0f / 180.0f * M_PI)) rotationY = (89.0f / 180.0f * M_PI);
            if (rotationY < -(89.0f / 180.0f * M_PI)) rotationY = -(89.0f / 180.0f * M_PI);
              
            //camera->lookingAt.x() = camera->position.x() + cos(rotationX) * cos(rotationY);
            //camera->lookingAt.y() = camera->position.y() + sin(rotationY);
            //camera->lookingAt.z() = camera->position.z() + sin(rotationX) * cos(rotationY);
            
            camera->update();
            
            clear_color(ic::Colors::blue);
            blinnPhongShader->use();
            woodTexture->use();
            camera->upload_to_shader(blinnPhongShader);
            blinnPhongShader->set_uniform_vec3f("viewPosition", camera->position);

            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    float x = i * 2.0f;
                    float z = j * 2.0f;

                    mesh->set_transformation(ic::Mat4x4().set_translation<3>({ x, 0.0f, z }));
                    mesh->draw(blinnPhongShader);
                }
            }

            return true; 
        }

        void dispose() override {
            blinnPhongShader->clear();
            mesh->dispose();
            woodTexture->dispose();
        }
};

int main(int argc, char *argv[]) {
    Example7 application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}