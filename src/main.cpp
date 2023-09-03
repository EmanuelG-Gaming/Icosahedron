// Examples (uncomment any of these)
/* Example 1: Displays a blue window on startup. */
/*
#include <Icosahedron/Core.h>

class Example1 : public ic::Application {
    public:
        bool init() override {
            // Use this to set up window settings, although this can also be done in the constructor
            displayName = "Example window";

            return true;
        }
        
        bool load() override {
            // This function is called after init() and after setting up the window

            return true;
        }

        void window_size_changed(int w, int h) override {
            // Called when the window is resized, and also if it is changed to fullscreen mode
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            // Called when events are retrieved and polled 

            return true;
        }

        bool update(float dt) override {
            // This is called once every frame
        
            clear_color(ic::Colors::blue);
            return true; 
        }

        void dispose() override {
            // Occurs when the application has to close

        }
};

int main(int argc, char *argv[]) {
    Example1 application;

    // Constructs a window that is 640 pixels wide and 480 pixels tall
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
    ic::Camera2D *camera;

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
            shape->add_attribute("textureCoords", 2, ic::GeometryGenerator::get().generate_UV_rectangle());
            shape->set_index_buffer({ 0, 1, 2, 0, 2, 3 });
            shape->set_material(ic::MeshMaterial2D(ic::Colors::white, 1.0f));

            texture = new ic::Texture<ic::T2D>("resources/textures/wood.png");
            shader = new ic::Shader(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D, false);

            camera = new ic::Camera2D();
        
            inputHandler.add_input((new ic::KeyboardController())->with_WASD(), "WASD");
            shapePosition = { 0.0f, 0.0f };

            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override {
            auto *controller = inputHandler.find_keyboard("WASD");
            ic::Vec2i dir = controller->get_direction();

            float speed = 1.0f;
            shapePosition.x() += dir.x() * speed * dt;
            shapePosition.y() += dir.y() * speed * dt;
            
            shape->set_transformation(ic::Mat4x4().set_translation(shapePosition));
            

            clear_color(ic::Colors::blue);
            
            shader->use();
            camera->use(shader);

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

struct RectangleShape {
    ic::Rectangle r;
    std::string atlasEntryName;

    RectangleShape(ic::Vec2f position, ic::Vec2f size, std::string atlasEntryName) : r(position, size), atlasEntryName(atlasEntryName) {

    }
};

class Example3 : public ic::Application {
    ic::Batch *batch, *textBatch;
    ic::TextureAtlas *texture;
    ic::TextAtlas *atlas;
    ic::Shader *shader, *textShader;
    ic::Camera2D *camera;

    RectangleShape *paddle1, *paddle2, *ball;
    ic::Vec2f vel;
    int points1, points2;

    public:
        bool init() override {
            displayName = "Example window";
            //scaling = ic::WindowScaling::fullscreen;

            return true;
        }
        
        bool load() override {
            shader = new ic::Shader(shaders.basicTextureShaderVertex2D, shaders.basicTextureShaderFrag2D, false);
            textShader = new ic::Shader(shaders.basicTextShaderVertex2D, shaders.basicTextShaderFrag2D, false);

            // We use the roboto font
            ic::FreeType::get().add_atlas("score", "resources/fonts/Roboto-Regular.ttf", 48);
            atlas = ic::FreeType::get().find_atlas("score");

            texture = new ic::TextureAtlas();
            texture->add_entries({ "paddle1", "resources/textures/white.png",
                                   "paddle2", "resources/textures/white.png",
                                   "ball", "resources/textures/ball.png" });

            batch = new ic::Batch(1000, ic::TRIANGLES);
            textBatch = new ic::Batch(1000, ic::TRIANGLES);
            
            camera = new ic::Camera2D();

            paddle1 = new RectangleShape({ -0.7f, 0.0f }, { 0.05f, 0.2f }, "paddle1");
            paddle2 = new RectangleShape({ 0.7f, 0.0f }, { 0.05f, 0.2f }, "paddle2");
            ball = new RectangleShape({ 0.0f, 0.0f }, { 0.05f, 0.05f }, "ball");

            points1 = 0;
            points2 = 0;

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
            auto *controller1 = inputHandler.find_keyboard("paddle1");
            auto *controller2 = inputHandler.find_keyboard("paddle2");
            
            ic::Vec2i dir1 = controller1->get_direction();
            ic::Vec2i dir2 = controller2->get_direction();

            
            paddle1->r.position.y() += dir1.y() * speed * dt;
            paddle2->r.position.y() += dir2.y() * speed * dt;

            paddle1->r.position.clamp({ -1.0f, -1.0f + paddle1->r.size.y() }, { 1.0f, 1.0f - paddle1->r.size.y() });
            paddle2->r.position.clamp({ -1.0f, -1.0f + paddle2->r.size.y() }, { 1.0f, 1.0f - paddle2->r.size.y() });
            
            // Rendering
            clear_color(ic::Colors::black);

            shader->use();
            camera->use(shader);
            texture->use();

            renderer.draw_rectangle(batch, texture->get_entry(paddle1->atlasEntryName),
                paddle1->r.position.x(), paddle1->r.position.y(), 
                paddle1->r.size.x(),     paddle1->r.size.y(), 
                ic::Colors::white);
            renderer.draw_rectangle(batch, texture->get_entry(paddle2->atlasEntryName),
                paddle2->r.position.x(), paddle2->r.position.y(), 
                paddle2->r.size.x(),     paddle2->r.size.y(), 
                ic::Colors::white);
            renderer.draw_rectangle(batch, texture->get_entry(ball->atlasEntryName),
                ball->r.position.x(), ball->r.position.y(), 
                ball->r.size.x(),     ball->r.size.y(), 
                ic::Colors::white);

            batch->render();

            // Text rendering
            textShader->use();
            camera->use(textShader);
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

        void restart(RectangleShape *winner) {
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

class Example4 : public ic::Application {
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
            shader = new ic::Shader(shaders.basicTextureShaderVertex2D, shaders.basicTextureShaderFrag2D, false);
            textShader = new ic::Shader(shaders.basicTextShaderVertex2D, shaders.basicTextShaderFrag2D, false);

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

int main(int argc, char *argv[]) {
    Example4 application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/

/* Example5: Polygon example. Demonstrates the use of per-vertex colors, separate texturing (no atlas), matrix transformations, and materials, all in the same shader program. */
/*
#include <Icosahedron/Core.h>

class Example5 : public ic::Application {
    ic::Mesh2D *mesh1;
    ic::Mesh2D *mesh2;

    ic::Texture<ic::T2D> *texture, *whiteTexture;
    ic::Camera2D *camera;

    ic::Shader *shader;
    float time;

    public:
        bool init() override {
            displayName = "Example window";
            scaling = ic::WindowScaling::fullscreen;

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
            
            texture = new ic::Texture<ic::T2D>("resources/textures/wood.png");
            whiteTexture = new ic::Texture<ic::T2D>("resources/textures/white.png");

            camera = new ic::Camera2D();

            time = 0.0f;

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
            camera->use(shader);

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

/** Example6: A scene that shows a rotating monkey head on a floor. */
#include <Icosahedron/Core.h>

std::string depthShaderVert = IC_ADD_GLSL_DEFINITION(
    layout (location = 0) in vec3 position;
    layout (location = 3) in vec3 normal;

    uniform mat4 lightSpaceMatrix;
    uniform mat4 model;
    uniform mat4 normalModel;

    out vec3 vPosition;
    out vec3 vNormal;
    
    void main() {
        vec4 pos = lightSpaceMatrix * model * vec4(position, 1.0);

        vPosition = pos.xyz;
        vNormal = vec3(normalModel * vec4(normal, 1.0));
        
        gl_Position = pos;

    }
);

std::string depthShaderFrag = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;
    in vec3 vNormal;

    void main() {
        vec3 normal = normalize(vNormal);
        vec3 lightDir = normalize(vec3(0.0, 0.0, 0.0) - vec3(-2.0, 3.0, -3.0));

        float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.001);
        //float bias = 0.01;

        //float depth = gl_FragCoord.z + (gl_FrontFacing ? bias : 0.0);
        float depth = gl_FragCoord.z + bias;
        gl_FragDepth = depth; 
    }
);

std::string vert = IC_ADD_GLSL_DEFINITION(
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    layout (location = 2) in vec2 tCoords;
    layout (location = 3) in vec3 normal;

    uniform mat4 projection = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    uniform mat4 view = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    uniform mat4 model = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    uniform mat4 normalModel = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    
    uniform mat4 lightSpaceMatrix;

    out vec3 vPosition;
    out vec3 vColor;
    out vec2 vTCoords;
    out vec3 vNormal;
    out vec4 vLightSpacePosition;

    void main() {
        vec4 pos = model * vec4(position, 1.0);

        vPosition = pos.xyz;
        vColor = color;
        vTCoords = tCoords;
        vNormal = vec3(normalModel * vec4(normal, 1.0));
        vLightSpacePosition = lightSpaceMatrix * pos;

        gl_Position = projection * view * pos;
    }
);

std::string fragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;
    in vec2 vTCoords;
    in vec3 vNormal;
    in vec4 vLightSpacePosition;

    struct PointLight {
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float constant;
        float linear;
        float quadratic;
    };

    PointLight l = PointLight(
        vec3(-2.0, 3.0, -3.0), 
        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.8, 0.7, 0.55), 
        
        1.0, 0.09, 0.032
    );
    
    uniform sampler2D sampleTexture;
    uniform sampler2D shadowMap;
    uniform vec3 viewPosition;

    out vec4 outColor;

    float compute_shadows(PointLight light, vec4 lightSpaceCoords) {
        // Perform perspective divide
        vec3 projCoords = lightSpaceCoords.xyz / lightSpaceCoords.w;
        // Convert to [0, 1] range
        projCoords = projCoords * 0.5 + 0.5;

        vec3 normal = normalize(vNormal);
        vec3 lightDir = normalize(light.position - vPosition);
 
        float currentDepth = projCoords.z;
        
        float result = 0.0;
        // Hard shadows
        //float closestDepth = texture(shadowMap, projCoords.xy).r;
        //result = currentDepth > closestDepth ? 0.85 : 0.0;

        // PCF soft shadows
        vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
        for (int i = -1; i <= 1; i++) for (int j = -1; j <= 1; j++) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(i, j) * texelSize).r;
            result += currentDepth > pcfDepth ? 0.85 : 0.0;
        }
        result /= 9.0;

        if (projCoords.z > 1.0) result = 0.0;

        return result;
    }

    vec4 compute_lighting(PointLight light) {
        float distance = length(light.position - vPosition);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        vec3 normal = normalize(vNormal);
        vec3 lightDirection = normalize(light.position - vPosition);
        vec3 viewDirection = normalize(viewPosition - vPosition);
        float dotProduct = dot(lightDirection, normal);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4 * attenuation;
        vec4 ambientColor = vec4(light.ambient, 1.0) * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0) * attenuation;
        vec4 diffuseColor = texture(sampleTexture, vTCoords);
        if (diffuseColor.a <= 0.1) discard; //diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
        else diffuseColor *= vec4(light.diffuse, 1.0) * diffuseIntensity;

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        vec4 specularColor = vec4(light.specular, 1.0) * specularIntensity;

        float shadow = compute_shadows(light, vLightSpacePosition);
        vec4 result = ambientColor + (1.0 - shadow) * (diffuseColor + specularColor);

        return result;
    }
    
    void main() {
        vec4 color = compute_lighting(l);
        outColor = color;
    }
);





class Example6 : public ic::Application {
    ic::Shader *shader, *depthShader;
    ic::Framebuffer *shadowMap;

    ic::Camera3D *camera;
    ic::Mesh3D *mesh, *floorMesh;
    ic::Texture<ic::T2D> *floorTexture, *whiteTexture;
    ic::OrbitalCameraController3D *controller;

    float time;
    int shadowWidth, shadowHeight;
            
    public:
        bool init() override {
            displayName = "Example window";
            scaling = ic::WindowScaling::fullscreen;
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            
            shader = new ic::Shader(vert, fragment, false);
            shader->use();
            shader->set_uniform_int("sampleTexture", 0);
            shader->set_uniform_int("shadowMap", 1);

            depthShader = new ic::Shader(depthShaderVert, depthShaderFrag, false);
            
            floorTexture = new ic::Texture<ic::T2D>("resources/textures/stone-bricks.png");
            whiteTexture = new ic::Texture<ic::T2D>("resources/textures/white.png");

            shadowWidth = 1024;
            shadowHeight = 1024;
            shadowMap = new ic::Framebuffer(ic::TEXTURE_ATTACH_DEPTH, ic::TEXTURE_DEPTH, shadowWidth, shadowHeight);

            //mesh = new ic::Mesh3D(ic::GeometryGenerator::get().generate_cube(0.5f));
            //mesh->jump_attribute();
            //mesh->add_attribute("textureCoords", 2, ic::GeometryGenerator::get().generate_UV_parallelipiped());
            //mesh->add_attribute("normal", 3, ic::GeometryGenerator::get().generate_normal_parallelipiped());
            //mesh->set_index_buffer(ic::GeometryGenerator::get().generate_parallelipiped_indices());


            //mesh = ic::OBJLoader::get().get_mesh(ic::File("resources/models/monkey.obj").get_path());
            mesh = ic::OBJLoader::get().get_mesh("resources/models/monkey.obj");

            floorMesh = new ic::Mesh3D(ic::GeometryGenerator::get().generate_parallelipiped(5.0f * 5, 0.1f, 5.0f * 5));
            floorMesh->jump_attribute();
            floorMesh->add_attribute("textureCoords", 2, ic::GeometryGenerator::get().generate_UV_parallelipiped(10.0f * 5, 0.2f, 10.0f * 5));
            floorMesh->add_attribute("normal", 3, ic::GeometryGenerator::get().generate_normal_parallelipiped());
            floorMesh->set_index_buffer(ic::GeometryGenerator::get().generate_parallelipiped_indices());
            floorMesh->set_transformation(ic::Mat4x4().set_translation<3>({0.0f, 0.0f, 0.0f}));

            

            camera = new ic::Camera3D();
            camera->position = { -3.0f, 1.5f, 0.0f };
            controller = new ic::OrbitalCameraController3D(camera, &inputHandler);
            //controller->flying = true;
            controller->center = { 0.0f, 0.5f, 0.0f };

            time = 0.0f;
            
            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            time += dt;
            
            controller->act(dt);
            camera->resize(IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            camera->update();

            // Light projection
            ic::Mat4x4 lightProj = ic::Mat4x4().set_orthographic(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 30.0f);
            ic::Mat4x4 lightView = ic::Mat4x4().set_look_at({ -2.0f * 2, 3.0f * 2, -3.0f * 2 }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
            ic::Mat4x4 lightSpaceMatrix = lightProj * lightView;

            // First pass - render to the depth map
            states.set_viewport(shadowWidth, shadowHeight);
            states.disable_face_culling();
            shadowMap->use();
            clear_color(ic::Colors::black);
            depthShader->use();
            depthShader->set_uniform_mat4("lightSpaceMatrix", lightSpaceMatrix);
            render_scene(depthShader);
            shadowMap->unuse();
            
            // Second pass - render scene using shadow map
            states.set_viewport(IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            states.enable_face_culling(ic::BACK, ic::CW);
            clear_color(ic::Colors::blue);

            shader->use();
            camera->upload_to_shader(shader);
            shader->set_uniform_mat4("lightSpaceMatrix", lightSpaceMatrix);
            shader->set_uniform_vec3f("viewPosition", camera->position);

            shadowMap->use_texture(1);
            render_scene(shader);

            return true; 
        }

        void render_scene(ic::Shader *passShader) {
            // Mesh
            //for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) for (int k = 0; k < 5; k++) {
            //    float x = i * 1.5f;
            //    float y = 0.6f + j * 1.5f;
            //    float z = k * 1.5f;
            //
            //    ic::Quaternion quat = ic::Quaternion().from_euler(0.0f, time, 0.0f);
            //    ic::Mat4x4 rotation = quat.to_rotation_matrix();
            //    ic::Mat4x4 translation = ic::Mat4x4().set_translation<3>({x, y, z});
            //
            //    mesh->set_transformation(translation * rotation);
            //    mesh->set_normal_transformation(rotation);
            //    whiteTexture->use();
            //    mesh->draw(passShader);
            //}

            ic::Quaternion quat = ic::Quaternion().from_euler(0.0f, time, 0.0f);
            ic::Mat4x4 rotation = quat.to_rotation_matrix();
            ic::Mat4x4 translation = ic::Mat4x4().set_translation<3>({0.0f, 0.6f, 0.0f});
            
            mesh->set_transformation(translation * rotation);
            mesh->set_normal_transformation(rotation);
            whiteTexture->use();
            mesh->draw(passShader);

            // Floor
            floorTexture->use();
            floorMesh->draw(passShader);
        }

        void dispose() override {
            shader->clear();
            depthShader->clear();

            mesh->dispose();
            floorMesh->dispose();

            floorTexture->dispose();
            whiteTexture->dispose();

            shadowMap->dispose();
        }
};

int main(int argc, char *argv[]) {
    Example6 application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}

/** Example7: A model viewer. Press up/down keys to increase/decrease the view radius.
 *  Use the Q key to switch between perspective and orthographic projections, and
 *  the R key to toggle the rotation of the viewed object.
*/
/*
#include <Icosahedron/Core.h>

std::string fragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;
    in vec2 vTCoords;
    in vec3 vNormal;

    struct PointLight {
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float constant;
        float linear;
        float quadratic;
    };
    PointLight l = PointLight(
        vec3(3.0, 3.0, 3.0), 
        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.8, 0.7, 0.55), 
        
        1.0, 0.09, 0.032
    );
    
    struct Material {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        float shininess;
    };

    uniform sampler2D sampleTexture;
    uniform Material material;
    uniform vec3 viewPosition;

    out vec4 outColor;
    vec4 compute_lighting(PointLight light) {
        float distance = length(light.position - vPosition);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        vec3 normal = normalize(vNormal);
        vec3 lightDirection = normalize(light.position - vPosition);
        vec3 viewDirection = normalize(viewPosition - vPosition);
        float dotProduct = dot(lightDirection, normal);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4 * attenuation;
        vec4 ambientColor = vec4(light.ambient * material.ambient, 1.0) * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0) * attenuation;
        vec4 diffuseColor = texture(sampleTexture, vTCoords);
        if (diffuseColor.a <= 0.1) diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
        else diffuseColor *= vec4(light.diffuse * (material.diffuse * diffuseIntensity), 1.0);

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), material.shininess / 4.0) * attenuation;
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess * 4.0) * attenuation;
        
        vec4 specularColor = vec4(light.specular * (material.specular * specularIntensity), 1.0);

        vec4 result = ambientColor + diffuseColor + specularColor;
        return result;
    }
    
    void main() {
        vec4 color = compute_lighting(l);
        outColor = color;
    }
);


class Example7 : public ic::Application {
    ic::Shader *shader;
    ic::Camera3D *camera;
    ic::Mesh3D *mesh;
    ic::Texture<ic::T2D> *whiteTexture;
    ic::OrbitalCameraController3D *controller;
    //ic::FreeRoamCameraController3D *controller;

    ic::OBJMaterialInfo material;

    float scale;
    float delta;
    float time;
    bool perspective;
    bool rotating;

    public:
        bool init() override {
            displayName = "Example window";
            scaling = ic::WindowScaling::fullscreen;
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            states.enable_face_culling(ic::FRONT, ic::CCW);

            shader = new ic::Shader(shaders.meshShaderVertex3D, fragment, false);
            whiteTexture = new ic::Texture<ic::T2D>("resources/textures/white.png");
            
            mesh = ic::OBJLoader::get().get_mesh("resources/models/boat.obj");
            mesh->set_transformation(ic::Mat4x4().set_translation<3>({0.0f, 0.0f, 0.0f}));

            material = ic::OBJLoader::get().get_materials("resources/models/icosahedron.mtl")["Material.001"];
            
            scale = 3.0f;
            perspective = true;
            rotating = false;
            delta = 0.0f;
            time = 0.0f;

            camera = new ic::Camera3D(perspective);
            camera->position = { -3.0f, 1.5f, 0.0f };
            controller = new ic::OrbitalCameraController3D(camera, &inputHandler);
            //controller = new ic::FreeRoamCameraController3D(camera, &inputHandler);
            //controller->flying = true;

            // Inputs
            auto scaling = new ic::KeyboardController();
            scaling->add_action([this](){ scale += 5 * delta; }, KEY_UP);
            scaling->add_action([this](){ scale -= 5 * delta; }, KEY_DOWN);

            inputHandler.add_input(scaling, "orbitScaling");

            auto toggle = new ic::KeyboardController();
            toggle->add_key_up_action([this](){ perspective = !perspective; }, KEY_Q);
            toggle->add_key_up_action([this](){ rotating = !rotating; }, KEY_R);
            
            inputHandler.add_input(toggle, "toggle");

            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            delta = dt;
            if (rotating) time += delta;
            if (scale < 0.2f) scale = 0.2f;

            camera->settings.perspective = perspective;
            controller->radius = scale;

            float div = perspective ? 1.0f : (1 / scale) * 800.0f;
            controller->act(dt);
            camera->resize(IC_WINDOW_WIDTH / div, IC_WINDOW_HEIGHT / div);
            camera->update();
            
            clear_color(ic::Colors::blue);
            shader->use();
            whiteTexture->use();
            camera->upload_to_shader(shader);

            shader->set_uniform_vec3f("viewPosition", camera->position);
            shader->set_uniform_vec3f("material.ambient", material.ambient);
            shader->set_uniform_vec3f("material.diffuse", material.diffuse);
            shader->set_uniform_vec3f("material.specular", material.specular);
            shader->set_uniform_float("material.shininess", material.shininess);
            
            
            //for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) {
            //    ic::Quaternion quat = ic::Quaternion().from_euler(0.0f, time, 0.0f);
            //    ic::Mat4x4 rotation = quat.to_rotation_matrix();
            //    ic::Mat4x4 translation = ic::Mat4x4().set_translation<3>({i * 2.0f, 0.0f, j * 2.0f});
            //    
            //    mesh->set_transformation(translation * rotation);
            //    mesh->set_normal_transformation(rotation);
            //    mesh->draw(shader);
            //}
            

            ic::Quaternion quat = ic::Quaternion().from_euler(time, time, 0.0f);
            ic::Mat4x4 rotation = quat.to_rotation_matrix();
               
            mesh->set_transformation(rotation);
            mesh->set_normal_transformation(rotation);
            mesh->draw(shader);

            return true; 
        }

        void dispose() override {
            shader->clear();
            mesh->dispose();
            whiteTexture->dispose();
        }
};

int main(int argc, char *argv[]) {
    Example7 application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}


/* Example8: An example that shows how "post-processing" can be achieved.
 * In this case, the effect is an approximation of a gaussian blur filter. */
/*
#include <Icosahedron/Core.h>

std::string screenVertex = IC_ADD_GLSL_DEFINITION(
    layout (location = 0) in vec2 position;
    layout (location = 1) in vec2 tCoords;

    out vec2 vPosition;
    out vec2 vTCoords;

    void main() {
        vPosition = position;
        vTCoords = vec2(tCoords.x, 1.0 - tCoords.y);
        
        gl_Position = vec4(vPosition, 0.0, 1.0);
    }
);

std::string screenFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec2 vPosition;
    in vec2 vTCoords;

    const float offset = 1.0 / 300.0;
    const int convolutionArea = 3 * 3;

    uniform sampler2D screenTexture;

    out vec4 outColor;

    void main() {
        vec2 offsets[convolutionArea] = vec2[](
            vec2(-offset, offset),
            vec2(0.0, offset),
            vec2(offset, offset),

            vec2(-offset, 0.0),
            vec2(0.0, 0.0),
            vec2(offset, 0.0),

            vec2(-offset, -offset),
            vec2(0.0, -offset),
            vec2(offset, -offset)
        );

        float convolution[convolutionArea] = float[](
            1.0 / 16.0, 1.0 / 8.0, 1.0 / 16.0,
            1.0 / 8.0, 1.0 / 4.0, 1.0 / 8.0,
            1.0 / 16.0, 1.0 / 8.0, 1.0 / 16.0
        );

        vec4 samples[9];
        vec4 color = vec4(0.0);

        for (int i = 0; i < convolutionArea; i++) {
            samples[i] = texture(screenTexture, vTCoords + offsets[i]);
        }
        for (int i = 0; i < convolutionArea; i++) {
            color += samples[i] * convolution[i];
        }

        outColor = color;
    }
);

std::string fragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;
    in vec2 vTCoords;
    in vec3 vNormal;
    struct PointLight {
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float constant;
        float linear;
        float quadratic;
    };
    PointLight l = PointLight(
        vec3(-2.0, 3.0, -3.0), 
        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.8, 0.7, 0.55), 
        
        1.0, 0.09, 0.032
    );
    
    uniform sampler2D sampleTexture;
    uniform vec3 viewPosition;
    out vec4 outColor;
    vec4 compute_lighting(PointLight light) {
        float distance = length(light.position - vPosition);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        vec3 normal = normalize(vNormal);
        vec3 lightDirection = normalize(light.position - vPosition);
        vec3 viewDirection = normalize(viewPosition - vPosition);
        float dotProduct = dot(lightDirection, normal);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4 * attenuation;
        vec4 ambientColor = vec4(light.ambient, 1.0) * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0) * attenuation;
        vec4 diffuseColor = texture(sampleTexture, vTCoords);
        if (diffuseColor.a <= 0.1) diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
        else diffuseColor *= vec4(light.diffuse, 1.0) * diffuseIntensity;

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        vec4 specularColor = vec4(light.specular, 1.0) * specularIntensity;
        vec4 result = ambientColor + diffuseColor + specularColor;
        return result;
    }
    
    void main() {
        vec4 color = compute_lighting(l);
        outColor = color;
    }
);


class Example8 : public ic::Application {
    ic::Shader *shader, *screenShader;
    ic::Framebuffer *framebuffer;
    
    ic::Camera3D *camera;
    ic::Mesh2D *screenQuad;
    ic::Mesh3D *mesh, *floorMesh;

    ic::Texture<ic::T2D> *floorTexture, *whiteTexture;
    ic::FreeRoamCameraController3D *controller;

    float time;
    public:
        bool init() override {
            displayName = "Example window";
            scaling = ic::WindowScaling::fullscreen;
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            
            shader = new ic::Shader(shaders.meshShaderVertex3D, fragment, false);
            screenShader = new ic::Shader(screenVertex, screenFragment, false);
            
            floorTexture = new ic::Texture<ic::T2D>("resources/textures/wood.png");
            whiteTexture = new ic::Texture<ic::T2D>("resources/textures/white.png");
            
            framebuffer = new ic::Framebuffer(ic::TEXTURE_ATTACH_COLOR_0, ic::TEXTURE_RGBA, IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);

            mesh = new ic::Mesh3D(ic::GeometryGenerator::get().generate_cube(0.5f));
            mesh->jump_attribute();
            mesh->add_attribute("textureCoords", 2, ic::GeometryGenerator::get().generate_UV_parallelipiped());
            mesh->add_attribute("normal", 3, ic::GeometryGenerator::get().generate_normal_parallelipiped());
            mesh->set_index_buffer(ic::GeometryGenerator::get().generate_parallelipiped_indices());
            
            
            floorMesh = new ic::Mesh3D(ic::GeometryGenerator::get().generate_parallelipiped(5.0f * 5, 0.1f, 5.0f * 5));
            floorMesh->jump_attribute();
            floorMesh->add_attribute("textureCoords", 2, ic::GeometryGenerator::get().generate_UV_parallelipiped(10.0f * 5, 0.2f, 10.0f * 5));
            floorMesh->add_attribute("normal", 3, ic::GeometryGenerator::get().generate_normal_parallelipiped());
            floorMesh->set_index_buffer(ic::GeometryGenerator::get().generate_parallelipiped_indices());
            floorMesh->set_transformation(ic::Mat4x4().set_translation<3>({0.0f, 0.0f, 0.0f}));

            screenQuad = new ic::Mesh2D(ic::GeometryGenerator::get().generate_rectangle(1.0f, 1.0f));
            screenQuad->add_attribute("textureCoords", 2, ic::GeometryGenerator::get().generate_UV_rectangle());
            screenQuad->set_index_buffer({ 0, 1, 2, 0, 2, 3 });

            camera = new ic::Camera3D();
            camera->position = { -3.0f, 1.5f, 0.0f };
            controller = new ic::FreeRoamCameraController3D(camera, &inputHandler);
            controller->flying = true;
            time = 0.0f;
            
            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        void window_size_changed(int w, int h) override {
            framebuffer->resize(w, h);
        }

        bool update(float dt) override {
            time += dt;

            controller->act(dt);
            camera->resize(IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            camera->update();
            
            // First pass - scene drawing
            framebuffer->use();
            clear_color(ic::Colors::blue);
            states.enable_face_culling(ic::FRONT, ic::CCW);
            
            shader->use();
            shader->set_uniform_vec3f("viewPosition", camera->position);
            camera->upload_to_shader(shader);
            
            whiteTexture->use();
            
            ic::Quaternion quat = ic::Quaternion().from_euler(0.0f, time, 0.0f);
            ic::Mat4x4 rotation = quat.to_rotation_matrix();
            ic::Mat4x4 translation = ic::Mat4x4().set_translation<3>({0.0f, 0.6f, 0.0f});
            mesh->set_transformation(translation * rotation);
            mesh->set_normal_transformation(rotation);
            mesh->draw(shader);

            floorTexture->use();
            floorMesh->draw(shader);
            
            framebuffer->unuse();


            // Second pass - drawing via framebuffer
            clear_color(ic::Colors::cyan);
            states.disable_face_culling();

            screenShader->use();
            framebuffer->use_texture();
            screenQuad->draw(screenShader);

            return true; 
        }

        void dispose() override {
            shader->clear();
            mesh->dispose();
            floorMesh->dispose();
            floorTexture->dispose();
            whiteTexture->dispose();
            framebuffer->dispose();
        }
};

int main(int argc, char *argv[]) {
    Example8 application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/

/* Example9: A very simple ray tracer. */
/*
#include <Icosahedron/Core.h>

const std::size_t RAYTRACING_WIDTH = 640;
const std::size_t RAYTRACING_HEIGHT = 480;

std::string rayVertex = IC_ADD_GLSL_DEFINITION(
    layout (location = 0) in vec2 position;
    layout (location = 1) in vec3 color;
    layout (location = 2) in vec2 tCoords;

    out vec2 vPosition;
    out vec2 vTCoords;

    void main() {
        vPosition = position;
        vTCoords = tCoords;
        
        gl_Position = vec4(vPosition, 0.0, 1.0);
    }
);

std::string rayFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;

    in vec2 vPosition;
    in vec2 vTCoords;

    struct RayResult {
        vec3 intersection;
        vec3 normal;
        float t;
        bool hit;
    };

    struct Ray {
        vec3 origin;
        vec3 direction;
    };
    
    
    struct Sphere {
        vec3 center;
        float radius;
        vec3 diffuse;
    };


    const int MAX_SPHERE_COUNT = 3;
    uniform Sphere spheres[MAX_SPHERE_COUNT];
    uniform vec3 lightPosition = vec3(1.0, 1.0, 1.0);
    uniform mat4 projection;


    RayResult ray_trace_sphere(Ray ray, Sphere sphere) {
        vec3 offset = ray.origin - sphere.center;

        // Form a 2nd order polynominal ax^2 + bx + c
        float a = dot(ray.direction, ray.direction);
        float b = 2.0 * dot(ray.direction, offset);
        float c = dot(offset, offset) - sphere.radius * sphere.radius;

        float discriminant = b*b - 4.0*a*c;
        
        RayResult result;
        if (discriminant > 0.0) {
            float alpha = (-b + sqrt(discriminant)) / (2.0 * a);
            float alpha2 = -b / a - alpha;
            if (abs(alpha2) < abs(alpha)) alpha = alpha2;
    
            result.intersection = ray.origin + ray.direction * alpha;
            result.normal = normalize(result.intersection - sphere.center);
            result.t = alpha;
            result.hit = true;

            return result;
        } else if (discriminant == 0.0) {
            // Tries to fix background noise
        
            float alpha = -b / (2.0 * a);
            float alpha2 = -b / a - alpha;
            if (abs(alpha2) < abs(alpha)) alpha = alpha2;
    
            result.intersection = ray.origin + ray.direction * alpha;
            result.normal = normalize(result.intersection - sphere.center);
            result.t = alpha;
            result.hit = true;

            return result;
        }

        result.hit = false;
        return result;
    }

    vec4 sphere_shading(RayResult intersection, Sphere sphere) {
        vec3 lightDir = normalize(lightPosition - intersection.intersection);
        vec3 normal = normalize(intersection.normal);
        float intensity = clamp(dot(lightDir, normal), 0.0, 1.0);

        vec3 diffuse = sphere.diffuse * intensity;
        vec4 color = vec4(diffuse, 1.0);

        return color;
    }


    vec4 ray_color(Ray ray, inout RayResult minIntersection) {
        float minimumT = 90000.0;
        vec4 color = vec4(0.0, 0.0, 1.0, 1.0);
        
        for (int i = 0; i < MAX_SPHERE_COUNT; i++) {
            Sphere base = spheres[i];

            RayResult rayRes = ray_trace_sphere(ray, base);
            if (rayRes.hit) {
                float intersectionT = rayRes.t;

                // Keep track of the minimum distance between the spheres and the ray position
                if (intersectionT < minimumT) {
                    minimumT = intersectionT;
                    color = sphere_shading(rayRes, base);
                    minIntersection = rayRes;
                }
            }
        }

        return color;    
    }

    vec4 reflect_ray(Ray ray, int iterations) {
        vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
        Ray currentRay = ray;

        for (int i = 0; i < iterations; i++) {
            RayResult intersection1;
            vec4 color1 = ray_color(currentRay, intersection1);
    
            vec3 reflectPosition = intersection1.intersection;
            vec3 reflectDirection = reflect(-ray.direction, intersection1.normal);
    
            RayResult intersection2;
            Ray reflected = Ray(reflectPosition, reflectDirection);
    
            vec4 color2 = ray_color(reflected, intersection2);
            sum += color1 + color2;
            currentRay = reflected;
        }
        sum /= (iterations * 2);

        return sum;
    }

    vec4 shadowing(Ray ray) {
        float minimumT = 90000.0;
        RayResult minRes;
        Sphere baseSphere;

        for (int i = 0; i < MAX_SPHERE_COUNT; i++) {
            Sphere base = spheres[i];

            RayResult rayRes = ray_trace_sphere(ray, base);
            if (!rayRes.hit) continue;
            float intersectionT = rayRes.t;

            // Keep track of the minimum distance between the spheres and the ray position
            if (intersectionT < minimumT) {
                minimumT = intersectionT;
                baseSphere = base;
                minRes = rayRes;
            }
        }

        vec3 biased = minRes.intersection + minRes.normal * 0.001;
        Ray shadowRay = Ray(biased, normalize(biased - lightPosition));
        for (int j = 0; j < MAX_SPHERE_COUNT; j++) {
            Sphere base2 = spheres[j];
            if (baseSphere == base2) continue;

            RayResult shadowRes = ray_trace_sphere(shadowRay, base2);
            if (shadowRes.hit) {
                return vec4(0.2, 0.2, 0.2, 1.0);
            }
        }

        return vec4(1.0, 1.0, 1.0, 1.0);
    }

    out vec4 outColor;

    void main() {
        vec2 centered = vTCoords * 2.0 - 1.0;

        mat4 inverseProj = inverse(projection);
        vec3 pos = vec3(centered.xy, 0.0);
        vec3 dir = normalize((inverseProj * vec4(pos, -1.0))).xyz;
        Ray ray = Ray(vec3(0.0, 0.0, 0.0), dir);

        vec4 average = reflect_ray(ray, 2);
        vec4 shadow = shadowing(ray);

        outColor = average * shadow;
    }
);


std::string screenFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec2 vPosition;
    in vec2 vTCoords;

    uniform sampler2D screenTexture;

    out vec4 outColor;

    void main() {
        vec4 color = texture(screenTexture, vTCoords);
        outColor = color;
    }
);

class Example9 : public ic::Application {
    ic::Shader *screenShader, *rayShader;
    ic::Framebuffer *framebuffer;
    
    ic::Mesh2D *screenQuad;
    ic::Camera3D *camera;
    float time;

    public:
        bool init() override {
            displayName = "Example window";
            
            return true;
        }
        
        bool load() override {
            rayShader = new ic::Shader(rayVertex, rayFragment, false);
            screenShader = new ic::Shader(shaders.meshShaderVertex2D, screenFragment, false);
            
            // Initialize these values directly
            rayShader->use();
            rayShader->set_uniform_vec3f("spheres[0].center", { 0.0f, 0.2f, 1.5f });
            rayShader->set_uniform_color("spheres[0].diffuse", ic::Colors::red);
            rayShader->set_uniform_float("spheres[0].radius", 0.05f);

            rayShader->set_uniform_vec3f("spheres[1].center", { 0.5f, 0.0f, 1.5f });
            rayShader->set_uniform_color("spheres[1].diffuse", ic::Colors::green);
            rayShader->set_uniform_float("spheres[1].radius", 0.35f);

            rayShader->set_uniform_vec3f("spheres[2].center", { -0.4f, -0.1f, 1.5f });
            rayShader->set_uniform_color("spheres[2].diffuse", ic::Colors::cyan);
            rayShader->set_uniform_float("spheres[2].radius", 0.5f);

            framebuffer = new ic::Framebuffer(ic::TEXTURE_ATTACH_COLOR_0, ic::TEXTURE_RGBA, RAYTRACING_WIDTH, RAYTRACING_HEIGHT);

            screenQuad = new ic::Mesh2D(ic::GeometryGenerator::get().generate_rectangle(1.0f, 1.0f));
            screenQuad->jump_attribute();
            screenQuad->add_attribute("textureCoords", 2, ic::GeometryGenerator::get().generate_UV_rectangle());
            screenQuad->set_index_buffer({ 0, 1, 2, 0, 2, 3 });

            camera = new ic::Camera3D();

            time = 0.0f;

            return true;
        }

        void window_size_changed(int w, int h) override {
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            
            return true;
        }

        bool update(float dt) override {
            time += dt;
            camera->update();

            clear_color(ic::Colors::blue);
            
            // First pass - getting raytraced information
            framebuffer->use();
            rayShader->use();
            rayShader->set_uniform_vec3f("lightPosition", { 0.5f, 0.1f, 0.0f });
            camera->upload_to_shader(rayShader);
            rayShader->set_uniform_vec3f("spheres[0].center", { ic::Mathf::get().sinf(time * 0.5f) * 0.3f, 0.0f, 0.5f });
            
            screenQuad->draw(rayShader);
            framebuffer->unuse();

            // Second pass - drawing the screen-covering quadrilateral
            screenShader->use();
            framebuffer->use_texture();
            screenQuad->draw(screenShader);

            return true; 
        }

        void dispose() override {
            screenShader->clear();
            rayShader->clear();

            framebuffer->dispose();
            screenQuad->dispose();
        }
};

int main(int argc, char *argv[]) {
    Example9 application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/