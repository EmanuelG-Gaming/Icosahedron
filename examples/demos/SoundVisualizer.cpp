#include <Icosahedron/Application.h>

#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Batch.h>

#include <Icosahedron/audio/Audio.h>
#include <Icosahedron/audio/Music.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>

#include <array>


class SoundVisualizer : public ic::Application {
    ic::Batch batch;
    ic::Shader shader;

    // Gaussian distribution weights, going from left to right or right to left.
    // Notice that the area under a section of this curve with constant length decreases as it moves away from the origin,
    // meaning that the change in overall effect is less noticeable as we add more weights.
    std::array<float, 32> weights;

    public:
        bool init() override {
            displayName = "Sound visualizer";

            return true;
        }
        
        bool load() override {
            // In this case, the standard deviation controls how "smooth" and less clustered the resulting frequency function would be
            float standardDeviation = 3.5f;
            
            float sqrt2PI = sqrt(2.0f * M_PI);
            for (int i = 0; i < weights.size(); i++) {
                float r = i / standardDeviation;
                float value = exp(-0.5f * r * r) / (standardDeviation * sqrt2PI);

                weights[i] = value;
            }
            
            shader = ic::ShaderLoader::get().load(shaders.basicShaderVertex2D, shaders.basicShaderFrag2D);

            batch = ic::Batch(100000, ic::TRIANGLES);

            ic::Audio::get().play(new ic::Music("resources/audio/insert music here.mp3"));
            
            return true;
        }


        bool handle_event(ic::Event event, float dt) override { 
            
            return true;
        }

        bool update(float dt) override {
            float *frequencies = ic::Audio::get().calculate_FFT();
            float *calculated = new float[256];

            // Applies a convolution
            for (int i = 0; i < 256; i++) {
                float value = frequencies[i] * weights[0];

                for (int j = 1; j < weights.size(); j++) {
                    value += get_frequency(frequencies, i + j) * weights[j];
                    value += get_frequency(frequencies, i - j) * weights[j];
                }

                calculated[i] = value;
            }

            // Draw height graph
            for (int i = 0; i < 256; i++) {
                float value = calculated[i] * 0.01f;
                float b = 256.0f - i * 1.5f;
                uint8_t brightness = (uint8_t) ic::Mathf::get().clamp(b, 0.0f, 256.0f);
                ic::Color color = { brightness, brightness, brightness };


                float x = -1.0f + i / (256.0f / 2.0f);
                float y = -1.0f + value / 2.0f;

                renderer.draw_rectangle(batch, x, y, 1.0f / 256.0f, value, color);
            }

            clear_color(ic::Colors::blue);

            shader.use();
            batch.render();


            return true; 
        }

        void dispose() override {
            shader.clear();
            batch.dispose();
        }

        float &get_frequency(float *frequencies, int index) {
            // Clamping to nearest element
            if (index < 0) return frequencies[0];
            if (index > 256 - 1) return frequencies[256 - 1];

            return frequencies[index];
        }
};

int main() {
    SoundVisualizer application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}