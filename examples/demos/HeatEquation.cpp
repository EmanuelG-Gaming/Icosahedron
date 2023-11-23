#include <Icosahedron/Application.h>

#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Batch.h>

#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>

#include <array>


const std::size_t TILE_WIDTH = 100, TILE_HEIGHT = 50;
const std::size_t TILE_AREA = TILE_WIDTH * TILE_HEIGHT;

// How fast heat spreads at a given moment
const float HEAT_COEFFICIENT = 25.0f;

const int SOLVER_TIME_STEPS = 5;
const float SOLVER_COOLDOWN = 0.05f;


/** The heat equation describes temperature transfer between points in space. */
class HeatEquation : public ic::Application {
    ic::Shader shader;
    ic::Batch batch;

    ic::Camera2D camera;

    std::array<float, TILE_WIDTH * TILE_HEIGHT> heatValues;
    float time;

    public:
        bool init() override {
            displayName = "Heat equation";

            return true;
        }
        
        bool load() override {
            memset(&heatValues, 0, sizeof(heatValues));
            
            shader = ic::ShaderLoader::get().load(shaders.basicShaderVertex2D, shaders.basicShaderFrag2D);
            batch = ic::Batch(1000000, ic::TRIANGLES);
            
            camera = ic::Camera2D();



            ic::InputHandler::get().add_input((new ic::KeyboardController())->with_WASD(), "WASD");

            ic::MouseController *controller = new ic::MouseController();
            controller->add_mouse_scroll_up_action([this]() { 
                float p = ic::InputHandler::get().find_mouse("mouse")->get_wheel_direction() * 0.05f;
                camera.scale = std::max(0.01f, std::min(camera.scale + p, 4.0f));
            });
            controller->add_mouse_scroll_down_action([this]() { 
                float p = ic::InputHandler::get().find_mouse("mouse")->get_wheel_direction() * 0.05f;
                camera.scale = std::max(0.01f, std::min(camera.scale + p, 4.0f));
            });

            controller->add_mouse_up_action([this]() { 
                ic::Vec2i p = ic::InputHandler::get().find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };

                ic::Vec2f levelPos = camera.unproject(pos);

                int x = (int) levelPos.x();
                int y = (int) levelPos.y();

                if (x < 0 || y < 0 || x >= TILE_WIDTH || y >= TILE_HEIGHT) return;

                heatValues[y * TILE_WIDTH + x] = 50.0f;
            });

            ic::InputHandler::get().add_input(controller, "mouse");

            time = 0.0f;

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

            auto *controller = ic::InputHandler::get().find_keyboard("WASD");
            ic::Vec2i dir = controller->get_direction();

            float speed = 5.0f;
            camera.position.x() += dir.x() * speed * dt;
            camera.position.y() += dir.y() * speed * dt;

            if (time >= SOLVER_COOLDOWN) {
                float timeStep = SOLVER_COOLDOWN / (float) SOLVER_TIME_STEPS;

                for (int s = 0; s < SOLVER_TIME_STEPS; s++) {
                    for (int i = 0; i < TILE_WIDTH; i++) {
                        for (int j = 0; j < TILE_HEIGHT; j++) {
                            ic::Vec2f gradient = get_double_gradient(i, j);  
                            heatValues[j * TILE_WIDTH + i] += (gradient.x() + gradient.y()) * HEAT_COEFFICIENT * timeStep;
                        }
                    }
                }

                time = 0.0f;
            }

            for (int i = 0; i < TILE_WIDTH; i++) {
                for (int j = 0; j < TILE_HEIGHT; j++) {
                    float x = i * 1.0f;
                    float y = j * 1.0f;

                    // Black-white colour representation
                    //float value = heatValues[j * TILE_WIDTH + i] * 255.0f;
                    //uint8_t colorValue = (uint8_t) ic::Mathf::get().clamp(value, 0.0f, 255.0f);
                    //ic::Color color = { colorValue, colorValue, colorValue };

                    // Heat colour grading
                    float value = heatValues[j * TILE_WIDTH + i];
                    float t = ic::Mathf::get().clamp(value, 0.0f, 1.0f);

                    ic::Color color;
                    if (t < 1.0f / 3.0f) {
                        color = ic::Colors::blue.interpolate(ic::Colors::green, t * 3.0f);
                    } else if (t >= 1.0f / 3.0f && t < 2.0f / 3.0f) {
                        color = ic::Colors::green.interpolate(ic::Colors::yellow, (t - 1.0f / 3.0f) * 3.0f);
                    } else {
                        color = ic::Colors::yellow.interpolate(ic::Colors::red, (t - 2.0f / 3.0f) * 3.0f);
                    }

                    renderer.draw_rectangle(batch, x, y, 0.5f, 0.5f, color);
                }
            }



            clear_color(ic::Colors::blue);

            shader.use();
            camera.use(shader);

            batch.render();
        
            return true; 
        }

        void dispose() override {
            shader.clear();
            batch.dispose();
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

        ic::Vec2f get_double_gradient(int x, int y) {
            ic::Vec2f result = {
                (get_heat(x + 1, y) + get_heat(x - 1, y) - 2.0f * get_heat(x, y)) / 1.0f,
                (get_heat(x, y + 1) + get_heat(x, y - 1) - 2.0f * get_heat(x, y)) / 1.0f
            };

            return result;
        }
};

int main(int argc, char *argv[]) {
    HeatEquation application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}