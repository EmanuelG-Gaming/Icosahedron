#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Texture.h>

#include <Icosahedron/scene/2d/Camera2D.h>
#include <Icosahedron/scene/2d/Mesh2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>

#include <array>

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
        float base = (v - 1.0) * 3.0;
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


const std::size_t TILE_WIDTH = 32, TILE_HEIGHT = 32;
const std::size_t TILE_AREA = TILE_WIDTH * TILE_HEIGHT;

const float HEAT_DIFFUSIVITY = 10.0f, WALL_TEMPERATURE = 30.0f, WALL_FLUX = 25.0f, WALL_DIFFUSIVITY = 100.0f;

const int SOLVER_TIME_STEPS = 20;
const float SOLVER_COOLDOWN = 0.01f;


/** The heat equation describes temperature transfer between points in space. */
class HeatEquation : public ic::Application {
    ic::Shader shader, textShader, wallShader;
    ic::Texture tileTexture;
    ic::Mesh2D tilesMesh;

    ic::Batch textBatch, wallBatch;
    ic::TextAtlas atlas;


    ic::Camera2D camera, uiCamera;

    std::array<float, TILE_AREA> heatValues;
    std::array<uint8_t, TILE_AREA> walls;

    float time;
    float averageHeat;
    uint8_t *data = new uint8_t[TILE_AREA];
            
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
            
            walls[0] = 1;
            for (int i = 0; i < 30; i++) {
                walls[5 * TILE_WIDTH + i + 5] = 1;
            }

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

            controller->add_mouse_hold_action([this]() { 
                ic::Vec2i p = ic::InputHandler::get().find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };
                ic::Vec2f levelPos = camera.unproject(pos);

                int x = (int) levelPos.x();
                int y = (int) (TILE_HEIGHT - levelPos.y());

                if (x < 0 || y < 0 || x >= TILE_WIDTH || y >= TILE_HEIGHT) return;

                heatValues[y * TILE_WIDTH + x] += 1000.0f;
            });

            ic::InputHandler::get().add_input(controller, "mouse");

            time = 0.0f;
            averageHeat = 0.0f;

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

            float speed = 50.0f;
            camera.position.x() += dir.x() * speed * dt;
            camera.position.y() += dir.y() * speed * dt;


            

            if (time >= SOLVER_COOLDOWN) {
                float timeStep = SOLVER_COOLDOWN / (float) SOLVER_TIME_STEPS;

                // Walls
                for (int j = 0; j < TILE_HEIGHT; j++) {
                    for (int i = 0; i < TILE_WIDTH; i++) {
                        int index = j * TILE_WIDTH + i;
    
                        if (wall_at_unsafe(i, j)) {
                            heatValues[index] = WALL_TEMPERATURE;
                        }
                    }
                }

                // Integration
                for (int j = 0; j < TILE_HEIGHT; j++) {
                    for (int i = 0; i < TILE_WIDTH; i++) {
                        int index = j * TILE_WIDTH + i;

                        for (int s = 0; s < SOLVER_TIME_STEPS; s++) {
                            ic::Vec2f gradient = get_heat_gradient(i, j);
                            ic::Vec2f doubleGradient = get_heat_double_gradient(i, j);
                            
                            if (wall_at_unsafe(i, j)) {
                                heatValues[index] += (gradient.x() + gradient.y()) * -WALL_FLUX / WALL_DIFFUSIVITY * timeStep;
                            }

                            heatValues[index] += (doubleGradient.x() + doubleGradient.y()) * HEAT_DIFFUSIVITY * timeStep;
                        }
                    }
                }


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

                time = 0.0f;
            }

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
            textBatch.render();


            return true; 
        }

        void dispose() override {
            shader.clear();
            textShader.clear();
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


        ic::Vec2f get_heat_gradient(int x, int y) {
            //if (wall_at(x, y)) {
            //    return { 0.0f, 0.0f };
            //}

            return {
                (get_heat(x + 1, y) - get_heat(x - 1, y)) * 0.5f,
                (get_heat(x, y + 1) - get_heat(x, y - 1)) * 0.5f
            };
        }

        ic::Vec2f get_heat_double_gradient(int x, int y) {
            //if (wall_at(x, y)) {
            //    return { 0.0f, 0.0f };
            //}

            float doubleHeat = 2.0f * get_heat(x, y);
            return {
                (get_heat(x + 1, y) + get_heat(x - 1, y) - doubleHeat),
                (get_heat(x, y + 1) + get_heat(x, y - 1) - doubleHeat)
            };
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