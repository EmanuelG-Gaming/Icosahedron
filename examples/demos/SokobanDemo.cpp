#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Batch.h>
#include <Icosahedron/graphics/gl/TextureAtlas.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>

#include <array>

const std::size_t WIDTH = 10, HEIGHT = 10;
const std::size_t AREA = WIDTH * HEIGHT;

const ic::Vec2i cardinalDirections[4] = {
    { 1, 0 },
    { 0, 1 },
    { -1, 0 },
    { 0, -1 },
};

class SokobanDemo : public ic::Application {
    ic::Batch tileBatch;
    ic::TextureAtlas tileAtlas;

    ic::Texture texture;
    ic::Camera2D camera;

    ic::Shader shader;
    float time;

    std::array<int, AREA> tileValues, boxes, overlays;

    ic::Vec2i playerPosition;
    bool mouseHeld;

    public:
        bool init() override {
            displayName = "Sokoban demo";
            
            return true;
        }
        
        bool load() override {
            memset(&tileValues, 0, sizeof(tileValues));
            memset(&overlays, 0, sizeof(overlays));
            memset(&boxes, 0, sizeof(boxes));

            tileValues = {
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                2, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                2, 2, 0, 0, 0, 0, 0, 2, 0, 2,
                2, 2, 1, 1, 1, 1, 1, 2, 0, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            };

            boxes = {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 1, 0, 1, 1, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
                0, 0, 0, 0, 0, 1, 1, 1, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            };

            overlays = {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            };

            tileBatch = ic::Batch(10000, ic::TRIANGLES);
            tileAtlas = ic::TextureAtlas(256, 256);
            tileAtlas.add_entry("white", "resources/textures/white.png");
            tileAtlas.add_entry("discontinuous-square", "resources/textures/discontinuous-square.png");
            tileAtlas.add_entry("circle", "resources/textures/ball.png");

            tileAtlas.add_entry("box", "resources/textures/box.png");
            tileAtlas.add_entry("wood", "resources/textures/wood.png");
            tileAtlas.add_entry("stone", "resources/textures/stone.png");
            tileAtlas.add_entry("stone-bricks", "resources/textures/stone-bricks.png");
            
            shader = ic::ShaderLoader::get().load(shaders.basicTextureShaderVertex2D, shaders.basicTextureShaderFrag2D);
            
            camera = ic::Camera2D();
            camera.scale = 0.1f;
            
            time = 0.0f;

            playerPosition = { 2, 1 };


            ic::MouseController *controller = new ic::MouseController();
            controller->add_mouse_scroll_up_action([this]() { 
                float p = ic::InputHandler::get().find_mouse("mouse")->get_wheel_direction() * 0.1f;
                camera.scale = std::max(0.1f, std::min(camera.scale + p, 4.0f));
            });
            controller->add_mouse_scroll_down_action([this]() { 
                float p = ic::InputHandler::get().find_mouse("mouse")->get_wheel_direction() * 0.1f;
                camera.scale = std::max(0.1f, std::min(camera.scale + p, 4.0f));
            });


            controller->add_mouse_down_action([this]() {
                ic::Vec2i p = ic::InputHandler::get().find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };

                ic::Vec2f levelPos = camera.unproject(pos);

                if (point_inside_rectangle(levelPos.x(), levelPos.y(), playerPosition.x(), playerPosition.y(), 1.5f, 1.5f)) {
                    mouseHeld = true;
                }
            });
            controller->add_mouse_up_action([this]() {
                if (!mouseHeld) return;

                ic::Vec2i p = ic::InputHandler::get().find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };

                ic::Vec2f levelPos = camera.unproject(pos);

                for (int i = 0; i < 4; i++) {
                    ic::Vec2i dir = cardinalDirections[i];

                    int nextPosX = playerPosition.x() + dir.x();
                    int nextPosY = playerPosition.y() + dir.y();

                    if (point_inside_rectangle(levelPos.x(), levelPos.y(), nextPosX, nextPosY, 0.25f, 0.25f)) {
                        move_by(dir.x(), dir.y());
                        break;
                    }
                }

                mouseHeld = !mouseHeld;
            });

            ic::InputHandler::get().add_input(controller, "mouse");


            ic::KeyboardController *keyboard = new ic::KeyboardController();
            keyboard->add_key_down_action([this]() {
                move_by(0, 1);
            }, KEY_W);
            keyboard->add_key_down_action([this]() {
                move_by(-1, 0);
            }, KEY_A);
            keyboard->add_key_down_action([this]() {
                move_by(0, -1);
            }, KEY_S);
            keyboard->add_key_down_action([this]() {
                move_by(1, 0);
            }, KEY_D);

            keyboard->add_key_down_action([this]() {
                move_by(0, 1);
            }, KEY_UP);
            keyboard->add_key_down_action([this]() {
                move_by(-1, 0);
            }, KEY_LEFT);
            keyboard->add_key_down_action([this]() {
                move_by(0, -1);
            }, KEY_DOWN);
            keyboard->add_key_down_action([this]() {
                move_by(1, 0);
            }, KEY_RIGHT);

            ic::InputHandler::get().add_input(keyboard, "WASD");



            mouseHeld = false;

            return true;
        }

        void window_size_changed(int w, int h) override {
            camera.width = w;
            camera.height = h;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            time += dt;
            
            camera.position.x() = playerPosition.x();
            camera.position.y() = playerPosition.y();

            // Rendering
            clear_color(ic::Colors::blue);
            
            shader.use();
            camera.use(shader);

            tileAtlas.use();
            
            // Draws textured tiles
            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    std::string entryName;
                    switch (tileValues[y * WIDTH + x]) {
                        case 0:
                            entryName = "wood";
                            break;
                        case 1:
                            entryName = "stone";
                            break;
                        case 2:
                            entryName = "stone-bricks";
                            break;
                    }

                    renderer.draw_rectangle(tileBatch, tileAtlas.get_entry(entryName), x, y, 0.5f, 0.5f);
                }
            }

            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    if (overlays[y * WIDTH + x] == 1) {
                        // Partitions
                        renderer.draw_rectangle(tileBatch, tileAtlas.get_entry("discontinuous-square"), x, y, 0.5f, 0.5f, ic::Colors::green);
                    }
                }
            }

            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    if (boxes[y * WIDTH + x] == 1) {
                        // Boxes
                        renderer.draw_rectangle(tileBatch, tileAtlas.get_entry("box"), x, y, 0.5f, 0.5f);
                    }
                }
            }

            // Player renderer
            renderer.draw_rectangle(tileBatch, tileAtlas.get_entry("white"), playerPosition.x(), playerPosition.y(), 0.5f, 0.5f, ic::Colors::yellow);

            // Mouse hold indicator
            if (mouseHeld) {
                for (int i = 0; i < 4; i++) {
                    ic::Vec2i dir = cardinalDirections[i];
                    int nextX = playerPosition.x() + dir.x();
                    int nextY = playerPosition.y() + dir.y();

                    if (!turn(nextX, nextY)) continue;

                    renderer.draw_rectangle(tileBatch, tileAtlas.get_entry("circle"), nextX, nextY, 0.25f, 0.25f);
                }
            }


            tileBatch.render();

            return true; 
        }

        void dispose() {
            shader.clear();
            tileAtlas.dispose();
            tileBatch.dispose();
        }


        bool turn(int newPlayerPositionX, int newPlayerPositionY) {
            if (is_wall_at(newPlayerPositionX, newPlayerPositionY)) {
                return false;
            }

            if (is_box_at(newPlayerPositionX, newPlayerPositionY)) {
                int dx = newPlayerPositionX - playerPosition.x();
                int dy = newPlayerPositionY - playerPosition.y();

                int nextPosX = newPlayerPositionX + dx;
                int nextPosY = newPlayerPositionY + dy;

                // Trivial case: don't push if there's a second box
                if (is_box_at(nextPosX, nextPosY) || is_wall_at(nextPosX, nextPosY)) {
                    return false;
                }
            }


            return true;
        }

        void move_by(int dx, int dy) {
            int nextPosX = playerPosition.x() + dx;
            int nextPosY = playerPosition.y() + dy;

            if (turn(nextPosX, nextPosY)) {
                playerPosition.x() = nextPosX;
                playerPosition.y() = nextPosY;

                if (is_box_at(playerPosition.x(), playerPosition.y())) {
                    boxes[playerPosition.y() * WIDTH + playerPosition.x()] = 0;
                    boxes[(playerPosition.y() + dy) * WIDTH + (playerPosition.x() + dx)] = 1;
                }
            }
        }


        bool is_wall_at(int x, int y) {
            return (tileValues[y * WIDTH + x] == 2);
        }

        bool is_box_at(int x, int y) {
            return (boxes[y * WIDTH + x]);
        }

        bool point_inside_rectangle(float x, float y, float rx, float ry, float width, float height) {
            return (x >= rx - width && x <= rx + width && y >= ry - height && y <= ry + height);
        }
};

/*
// Benchmark results (debug mode):
// 10x10 map: 1960 FPS
// 50x50 map: 352 FPS
// 100x100 map: 105 FPS
// 200x200 map: 27 FPS
// 500x500 map: 5 FPS
// 1000x1000 map: 1-2 FPS
class SokobanDemoBatched : public ic::Application {
    ic::Batch tileBatch;

    ic::Texture texture;
    ic::Camera2D camera;

    ic::Shader shader;
    float time;

    int fps;
    float fpsCounter, lastFPSTime;

    public:
        bool init() override {
            displayName = "Sokoban demo";
            
            return true;
        }
        
        bool load() override {
            tileBatch = ic::Batch(1000000000, ic::TRIANGLES);

            shader = ic::ShaderLoader::get().load(shaders.basicTextureShaderVertex2D, shaders.basicTextureShaderFrag2D);
            texture = ic::TextureLoader::get().load_png("resources/textures/wood.png");
            
            camera = ic::Camera2D();
            camera.scale = 0.5f;
            
            time = 0.0f;

            fps = 0;
            fpsCounter = lastFPSTime = 0.0f;

            set_window_vsync(0);

            return true;
        }

        void window_size_changed(int w, int h) override {
            camera.width = w;
            camera.height = h;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            std::cout << "Frame rate: " << fps << " FPS" << "\n";
            time += dt;

            clear_color(ic::Colors::blue);
            
            shader.use();
            camera.use(shader);


            // Draws textured tiles
            texture.use();
            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    renderer.draw_rectangle(tileBatch, (float) x, (float) y, 0.5f, 0.5f);
                }
            }
            tileBatch.render();

            calculate_FPS(fps);
            
            return true; 
        }

        void dispose() {
            shader.clear();
            texture.dispose();
            tileBatch.dispose();
        }

        void calculate_FPS(int &to) {
            float current = SDL_GetTicks() * 0.001f;
            fpsCounter++;

            if (current - lastFPSTime > 1.0f) {
                lastFPSTime = current;
                to = (int) fpsCounter;
                fpsCounter = 0.0f;
            }
        }
};


// Benchmark results (also in debug mode):
// 10x10 map: ~1900 FPS
// 50x50 map: 290 FPS
// 100x100 map: 83 FPS
// 200x200 map: 22 FPS
// 500x500 map: 3-4 FPS
// 1000x1000 map: 1 FPS
class SokobanDemoMeshes : public ic::Application {
    ic::Mesh2D tileMesh;

    ic::Texture texture;
    ic::Camera2D camera;

    ic::Shader shader;
    float time;

    int fps;
    float fpsCounter, lastFPSTime;

    public:
        bool init() override {
            displayName = "Sokoban demo";
            
            return true;
        }
        
        bool load() override {
            tileMesh = ic::GeometryGenerator::get().generate_rectangle_mesh(0.5f, 0.5f);

            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D);
            texture = ic::TextureLoader::get().load_png("resources/textures/wood.png");
            
            camera = ic::Camera2D();
            camera.scale = 0.5f;
            
            time = 0.0f;

            fps = 0;
            fpsCounter = lastFPSTime = 0.0f;

            set_window_vsync(0);

            return true;
        }

        void window_size_changed(int w, int h) override {
            camera.width = w;
            camera.height = h;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            std::cout << "Frame rate: " << fps << " FPS" << "\n";
            time += dt;

            clear_color(ic::Colors::blue);
            
            shader.use();
            camera.use(shader);


            // Draws textured tiles
            texture.use();
            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    ic::Mat4x4 translation = ic::Mat4x4().set_translation<2>({ (float) x, (float) y });

                    tileMesh.set_transformation(translation);
                    tileMesh.draw(shader);
                }
            }

            calculate_FPS(fps);
            
            return true; 
        }

        void dispose() {
            shader.clear();
            texture.dispose();
            tileMesh.dispose();
        }

        void calculate_FPS(int &to) {
            float current = SDL_GetTicks() * 0.001f;
            fpsCounter++;

            if (current - lastFPSTime > 1.0f) {
                lastFPSTime = current;
                to = (int) fpsCounter;
                fpsCounter = 0.0f;
            }
        }
};
*/

int main() {
    SokobanDemo application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}