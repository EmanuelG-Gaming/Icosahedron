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
    ic::Batch2D *batch;
    ic::Texture<ic::T2D> *texture;
    ic::Shader *shader;

    ic::RectangleShape *shape;
    
    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }
        
        bool load() override {
            batch = new ic::Batch2D(1000, ic::TRIANGLES);
            shape = new ic::RectangleShape();
            texture = new ic::Texture<ic::T2D>({"resources/textures/wood.png"});
            shader = shaders.basicTextureShader2D;

            inputHandler.add_input(new ic::WASDController(), "WASD");

            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override { 
            float speed = 1.0f;
            ic::KeyboardController *controller = (ic::KeyboardController*) inputHandler.find_input("WASD");
            ic::Vec2i dir = controller->direction;

            shape->r.position.x() += dir.x() * speed * dt;
            shape->r.position.y() += dir.y() * speed * dt;


            clear_color(ic::Colors::blue);

            shader->use();
            texture->use();
            shape->draw(renderer, batch, ic::Colors::white);
            batch->render();

            return true; 
        }

        void dispose() override {
            batch->dispose();
            texture->dispose();
            shader->clear();
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
            return true;
        }
        
        bool load() override {
            shader = shaders.basicTextureShader2D;
            textShader = shaders.basicTextShader2D;

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

#include <Icosahedron/Core.h>

const std::size_t MAP_WIDTH = 16;
const std::size_t MAP_HEIGHT = 16;
const std::size_t MAP_AREA = MAP_WIDTH * MAP_HEIGHT;
const int DROP_DISPARITY = 5;

const std::array<int, MAP_AREA> tiles = {
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
        this->shape = new ic::PolygonShape(ic::GeometryGenerator::generate_regular_polygon(3));
        this->hitbox = ic::Rectangle({ 0.0f, 0.0f }, { 0.8f, 0.8f });
    }

    Drop(ic::Vec2f position, int sides, float radius) {
        this->shape = new ic::PolygonShape(ic::GeometryGenerator::generate_regular_polygon(sides, radius), position);
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

    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }
        
        bool load() override {
            shader = shaders.basicTextureShader2D;
            textShader = shaders.basicTextShader2D;

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

            shape = new ic::RectangleShape({ 0.0f, 0.0f }, { 0.4f, 0.4f }, "player");
            shape->set_atlas(texture);

            for (int x = 0; x < MAP_WIDTH; x++) {
                for (int y = 0; y < MAP_HEIGHT; y++) {
                    int index = y * MAP_WIDTH + x;
                    if (obstructing[index]) continue;
                    
                    if (rand() % DROP_DISPARITY == 1) {
                        drops.push_back(new Drop({ 0.0f + x, 0.0f + y }, rand() % 5 + 3, (rand() % 50 / 300.0f) + 0.1f));
                    }
                }
            }

            ic::KeyboardController *debugCont = new ic::KeyboardController();
            debugCont->add_key_up_action([this]() {collisionDebug = !collisionDebug;}, KEY_T);
            
            inputHandler.add_input((new ic::KeyboardController())->with_WASD(), "WASD");
            inputHandler.add_input(debugCont, "collisionDebug");

            collisionDebug = false;
            collected = 0;
            initialSize = drops.size();

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
            
            textBatch->render();

            return true; 
        }

        void dispose() override {
            batch->dispose();
            texture->dispose();
            shader->clear();
            textShader->clear();
        }

        void collect(Drop *drop) {
            for (int i = 0; i < drops.size(); i++) {
                if (drop != drops[i]) continue;
                drops.erase(drops.begin() + i);
                delete drop;
                collected++;

                return;
            }
        }
};

int main(int argc, char *argv[]) {
    Example4 application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}