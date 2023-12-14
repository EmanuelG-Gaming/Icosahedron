#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Texture.h>

#include <Icosahedron/math/geom/Rasterization.h>

#include <Icosahedron/scene/2d/Camera2D.h>
#include <Icosahedron/scene/2d/Mesh2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>

#include <array>
#include <thread>
#include <mutex>


std::string fragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;

    in vec2 vTCoords;

    uniform sampler2D sampleTexture;

    out vec4 outColor;

    const vec3 c3 = vec3(1.0, 0.2, 0.0);
    const vec3 c2 = vec3(0.0, 0.7, 0.0);
    const vec3 c1 = vec3(0.0, 0.0, 0.5);


    float towards_c2(float v) {
        // Uses a gaussian distribution
        float base = (v - 1.0) * 2.5;
        return exp(-(base * base));
    }
    vec3 get_color_from_value(float v) {
        vec3 color1 = mix(c1, c2, towards_c2(v));
        vec3 color2 = mix(c2, c3, v);
        return mix(color1, color2, v);
    }
    

    void main() {
        vec3 color = get_color_from_value(texture(sampleTexture, vTCoords).r);
        outColor = vec4(color, 1.0);
    }
);


const std::size_t TILE_WIDTH = 16, TILE_HEIGHT = 16;
const std::size_t TILE_AREA = TILE_WIDTH * TILE_HEIGHT;

const float HEAT_DIFFUSIVITY = 5.0f, WALL_TEMPERATURE = 28.0f;

const int SOLVER_TIME_STEPS = 18;
const int SOLVER_DELTA_TIME = 1;

const int NUMBER_OF_THREADS = 2;


/** The heat equation describes temperature transfer between points in space. 
 *  In this case, the application uses the finite difference method, which is essentially
 *  approximating the surface as a grid of number values, then calculating the derivatives easily 
 *  at every grid point.
*/
class HeatEquation : public ic::Application {
    ic::Shader shader, textShader, wallShader;
    ic::Texture tileTexture;
    ic::Mesh2D tilesMesh;

    ic::Batch textBatch, wallBatch;
    ic::TextAtlas atlas;


    ic::Camera2D camera, uiCamera;

    std::array<float, TILE_AREA> heatValues;
    std::array<uint8_t, TILE_AREA> walls;

    std::vector<std::thread> threads;
    std::mutex threadMutex;

    float time;
    float averageHeat;
    uint8_t *data = new uint8_t[TILE_AREA];
        
    ic::Vec2i previousPosition, previousWallPosition;

    float deltaTime;
    bool threadsRunning;

    public:
        bool init() override {
            displayName = "Heat equation";

            return true;
        }
        
        bool load() override {
            // Rendering optimization: Use textures instead of batches to represent heat tiles
            // Well, the walls are drawn using a batch, but there are usually fewer walls than heat tiles

            memset(&heatValues, 0, sizeof(heatValues));
            memset(&walls, 0, sizeof(walls));

            // Set up a blank texture from an array
            ic::TextureParameters params;
            params.magFilter = params.minFilter = ic::TEXTURE_FILTER_LINEAR;
            params.wrapU = params.wrapV = ic::TEXTURE_WRAP_CLAMP_TO_EDGE;

            tileTexture = ic::Texture().setup_from_array(heatValues.data(), TILE_WIDTH, TILE_HEIGHT, GL_RED, GL_RED, params);
           
            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, fragment);
            textShader = ic::ShaderLoader::get().load(shaders.basicTextShaderVertex2D, shaders.basicTextShaderFrag2D);
            wallShader = ic::ShaderLoader::get().load(shaders.basicShaderVertex2D, shaders.basicShaderFrag2D);
             
            
            textBatch = ic::Batch(10000, ic::TRIANGLES);
            wallBatch = ic::Batch(100000, ic::TRIANGLES);
            ic::FreeType::get().add_atlas("score", "resources/fonts/Roboto-Regular.ttf", 48);
            atlas = ic::FreeType::get().find_atlas("score");



            camera = ic::Camera2D();
            uiCamera = ic::Camera2D();

            tilesMesh = ic::GeometryGenerator::get().generate_rectangle_mesh(TILE_WIDTH / 2.0f, TILE_HEIGHT / 2.0f);
            tilesMesh.set_transformation(ic::Mat4x4().set_translation<2>({ TILE_WIDTH / 2.0f, TILE_HEIGHT / 2.0f }));


            ic::MouseController *controller = new ic::MouseController();
            controller->add_mouse_scroll_up_action([this]() { 
                float p = ic::InputHandler::get().find_mouse("mouse")->get_wheel_direction() * 0.05f;
                camera.scale = std::max(0.01f, std::min(camera.scale + p, 4.0f));
            });
            controller->add_mouse_scroll_down_action([this]() { 
                float p = ic::InputHandler::get().find_mouse("mouse")->get_wheel_direction() * 0.05f;
                camera.scale = std::max(0.01f, std::min(camera.scale + p, 4.0f));
            });

            controller->add_mouse_down_action([this]() { 
                ic::Vec2i p = ic::InputHandler::get().find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };
                ic::Vec2f levelPos = camera.unproject(pos);

                int x = (int) levelPos.x();
                int y = (int) (TILE_HEIGHT - levelPos.y());

                previousPosition = { x, y };
            });

            controller->add_mouse_hold_action([this]() { 
                ic::Vec2i p = ic::InputHandler::get().find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };
                ic::Vec2f levelPos = camera.unproject(pos);

                int x = (int) levelPos.x();
                int y = (int) (TILE_HEIGHT - levelPos.y());

                ic::Rasterization::get().line(x, y, previousPosition.x(), previousPosition.y(), [this](int i, int j) {
                    if (i < 0 || j < 0 || i >= TILE_WIDTH || j >= TILE_HEIGHT) return;
                    heatValues[j * TILE_WIDTH + i] += 10000000.0f;
                });

                previousPosition = { x, y };
            });

            ic::InputHandler::get().add_input(controller, "mouse");



            ic::KeyboardController *keyboard = (new ic::KeyboardController())->with_WASD();

            // These have the same location
            auto move_to = [this]() {
                ic::Vec2i p = ic::InputHandler::get().find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };
                ic::Vec2f levelPos = camera.unproject(pos);

                int x = (int) levelPos.x();
                int y = (int) (TILE_HEIGHT - levelPos.y());

                previousWallPosition = { x, y };
            };

            keyboard->add_key_down_action(move_to, KEY_L);
            keyboard->add_key_down_action(move_to, KEY_M);

            keyboard->add_action([this]() { 
                ic::Vec2i p = ic::InputHandler::get().find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };
                ic::Vec2f levelPos = camera.unproject(pos);

                int x = (int) levelPos.x();
                int y = (int) (TILE_HEIGHT - levelPos.y());

                ic::Rasterization::get().line(x, y, previousWallPosition.x(), previousWallPosition.y(), [this](int i, int j) {
                    if (i < 0 || j < 0 || i >= TILE_WIDTH || j >= TILE_HEIGHT) return;
                    walls[j * TILE_WIDTH + i] = 1;
                });

                previousWallPosition = { x, y };
            }, KEY_L);

            keyboard->add_action([this]() { 
                ic::Vec2i p = ic::InputHandler::get().find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };
                ic::Vec2f levelPos = camera.unproject(pos);

                int x = (int) levelPos.x();
                int y = (int) (TILE_HEIGHT - levelPos.y());

                ic::Rasterization::get().line(x, y, previousWallPosition.x(), previousWallPosition.y(), [this](int i, int j) {
                    if (i < 0 || j < 0 || i >= TILE_WIDTH || j >= TILE_HEIGHT) return;
                    walls[j * TILE_WIDTH + i] = 0;
                    heatValues[j * TILE_WIDTH + i] = 0.0f;
                });

                previousWallPosition = { x, y };
            }, KEY_M);

            ic::InputHandler::get().add_input(keyboard, "WASD");


            time = 0.0f;
            averageHeat = 0.0f;
            deltaTime = 0.0f;
            threadsRunning = true;


            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            for (int i = 0; i < NUMBER_OF_THREADS; i++) {
                int width = TILE_WIDTH / NUMBER_OF_THREADS;
                int height = TILE_HEIGHT;
                int x = i * (width);
                int y = 0;

                threads.push_back(std::thread([&](int topLeftX, int topLeftY, int w, int h) {
                    while (threadsRunning) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(SOLVER_DELTA_TIME));
                        std::lock_guard<std::mutex> guard(threadMutex);
                        step(topLeftX, topLeftY, w, h);
                    }
                }, x, y, width, height));

            }

            

            return true;
        }

        void window_size_changed(int w, int h) override {
            camera.resize(w, h);
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            time += dt;
            deltaTime = dt;

            auto *controller = ic::InputHandler::get().find_keyboard("WASD");
            ic::Vec2i dir = controller->get_direction();

            float speed = 50.0f;
            camera.position.x() += dir.x() * speed * dt;
            camera.position.y() += dir.y() * speed * dt;


        
            // Currently a very simple square prism integration here
            averageHeat = 0.0f;
            for (int i = 0; i < TILE_AREA; i++) {
                averageHeat += heatValues[i];
            }
            averageHeat /= TILE_AREA;

            for (int i = 0; i < TILE_AREA; i++) {
                data[i] = (uint8_t) ic::Mathf::get().clamp(heatValues[i], 0.0f, 255.0f);
            }
            tileTexture.set_pixel_content(data, GL_RED);


            // Rendering
            clear_color(ic::Colors::blue);

            shader.use();
            camera.use(shader);
            
            tileTexture.use();
            tilesMesh.draw(shader);


            // Walls
            wallShader.use();
            camera.use(wallShader);

            for (int j = 0; j < TILE_HEIGHT; j++) {
                for (int i = 0; i < TILE_WIDTH; i++) {
                    int index = j * TILE_WIDTH + i;
                
                    if (walls[index]) {
                        float x = i * 1.0f + 0.5f;
                        float y = (TILE_HEIGHT - 1) - j * 1.0f + 0.5f;
                        renderer.draw_rectangle(wallBatch, x, y, 0.5f, 0.5f, ic::Colors::gray);
                        renderer.draw_rectangle(wallBatch, x, y, 0.4f, 0.4f, ic::Colors::lightGray);
                    }
                }
            }
            wallBatch.render();

            // Text rendering
            textShader.use();
            atlas.use();

            uiCamera.use(textShader);
            renderer.draw_string(textBatch, atlas, "Average heat: " + std::to_string(averageHeat) + " Celsius", -1.2f, 0.9f);
            renderer.draw_string(textBatch, atlas, "Use the mouse buttons to add heat,", -1.2f, 0.8f, 0.7f, 0.7f);
            renderer.draw_string(textBatch, atlas, "and the L/M keys to place/remove walls.", -1.2f, 0.72f, 0.7f, 0.7f);
            textBatch.render();


            return true;
        }

        void step(int topLeftX, int topLeftY, int width, int height) {
            std::cout << "Index: " << std::this_thread::get_id() << "\n";
            
            // Integration
            float timeStep = 0.01f / (float) SOLVER_TIME_STEPS;

            for (int j = topLeftY; j < height; j++) {
                for (int i = topLeftX; i < width; i++) {
                    int index = j * TILE_WIDTH + i;

                    for (int s = 0; s < SOLVER_TIME_STEPS; s++) {
                        if (wall_at(i, j)) {
                            heatValues[index] = WALL_TEMPERATURE;
                        }
                        else {
                            float doubleGradientX = 0.0f, doubleGradientY = 0.0f;
                            int checkX = 0, checkY = 0;
                            
                            if (!wall_at(i + 1, j)) {
                                doubleGradientX += get_heat(i + 1, j);
                                checkX++;
                            }
                            if (!wall_at(i - 1, j)) {
                                doubleGradientX += get_heat(i - 1, j);
                                checkX++;
                            }
                            if (!wall_at(i, j + 1)) {
                                doubleGradientY += get_heat(i, j + 1);
                                checkY++;
                            }
                            if (!wall_at(i, j - 1)) {
                                doubleGradientY += get_heat(i, j - 1);
                                checkY++;
                            }

                            float currentHeat = get_heat_unsafe(i, j);
                            doubleGradientX -= currentHeat * checkX;
                            doubleGradientY -= currentHeat * checkY;

                            heatValues[index] += (doubleGradientX + doubleGradientY) * HEAT_DIFFUSIVITY * timeStep;
                        }

                        if (!threadsRunning) {
                            return;
                        }
                    }
                }
            }
        }

        void dispose() override {
            shader.clear();
            textShader.clear();

            threadsRunning = false;
            for (auto &thread : threads) {
                thread.join();
            }
        }

        float get_heat_unsafe(int x, int y) {
            return heatValues[y * TILE_WIDTH + x];
        }

        float get_heat(int x, int y) {
            // Clamp to edge, so that the derivative at that point is 0
            if (x < 0) return get_heat_unsafe(0, y);
            if (x >= TILE_WIDTH) return get_heat_unsafe(TILE_WIDTH - 1, y);
            if (y < 0) return get_heat_unsafe(x, 0);
            if (y >= TILE_HEIGHT) return get_heat_unsafe(x, TILE_HEIGHT - 1);

            return get_heat_unsafe(x, y);
        }

        int wall_at_unsafe(int x, int y) {
            return walls[y * TILE_WIDTH + x];
        }

        int wall_at(int x, int y) {
            if (x < 0 || y < 0 || 
               x >= TILE_WIDTH || y >= TILE_HEIGHT) return 0;
            
            return wall_at_unsafe(x, y);
        }
};

int main(int argc, char *argv[]) {
    HeatEquation application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}