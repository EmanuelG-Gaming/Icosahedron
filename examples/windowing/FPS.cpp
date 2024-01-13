#include <Icosahedron/Application.h>

#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Batch.h>

#include <Icosahedron/graphics/TextAtlas.h>

#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>


/** @brief Shows how to get the frames per second (FPS) of a window, and the output as text. */
class FPS : public ic::Application {
    ic::Batch batch;
    ic::Shader shader;
    ic::Camera2D camera;
    ic::TextAtlas atlas;

    int fps;
    float fpsCounter, lastFPSTime;

    public:
        bool init() override {
            displayName = "Frames per second";

            return true;
        }
        
        bool load() override {
            shader = ic::ShaderLoader::get().load(shaders.basicTextShaderVertex2D, shaders.basicTextShaderFrag2D);
            batch = ic::Batch(10000, ic::TRIANGLES);

            camera = ic::Camera2D();

            atlas = ic::FreeType::get().add_atlas("font", "resources/fonts/Roboto-Regular.ttf", 40);

            fps = 0;
            fpsCounter = lastFPSTime = 0.0f;

            ///// VSync settings /////
            // VSync can give radically different FPS, depending on whether or not it's toggled.
            // On one side, disabling VSync can give you a lot more FPS, but it makes your PC use a lot more for a single app (even 98% usage on the GPU), because
            // it causes "screen tearing", where the GPU renders many similar pictures on a single frame, which causes small artifacts in the final image.

            // My previous solution was to just delay the application's execution at the end of each frame by some amount
            
            //set_window_vsync(0);

            //set_window_vsync(1);
            //set_window_vsync(-1);


            return true;
        }

        void window_size_changed(int w, int h) override {

        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            renderer.draw_string_centered(batch, atlas, "Frame rate: " + std::to_string(fps) + " FPS", 0.0f, 0.0f, 2.0f, 2.0f);

            clear_color(ic::Colors::blue);

            shader.use();
            camera.use(shader);
            batch.render();

            calculate_FPS(fps);
        
            return true; 
        }

        void dispose() override {
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

int main() {
    FPS application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}