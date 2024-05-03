#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/Image.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>
#include <Icosahedron/util/Noise.h>
#include <IcosahedronDebug/ConsoleOutput.h>

// ic::Shaders::basicTextureShaderFrag2D with slight modifications to allow more "refined" alpha blending,
// rather than a cheap fragment discard when the color reaches a certain threshold
std::string alphaBlendingFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;

    in vec3 vColor;
    in vec2 vTCoords;

    uniform sampler2D sampleTexture;

    out vec4 outColor;

    void main() {
        vec4 color = texture(sampleTexture, vTCoords);
        outColor = color;
    }
);

/* Shows how to achieve alpha transparency by using a loaded .png texture and a procedural image. */
class Transparency2D : public ic::Application {
    ic::Image image;
                
    ic::Mesh2D mesh1, mesh2;

    ic::Texture texture1, texture2;
    ic::Camera2D camera;

    ic::Shader shader;

    public:
        bool init() override {
            engine.window.set_title("2D transparency");
            
            return true;
        }
        
        bool load() override {
            // Alpha blending
            ic::GL::enable_blending(ic::SRC_ALPHA, ic::DEST_ONE_MINUS_SRC_ALPHA);

            image = ic::Image(64, 64);

            // Mesh 1
            mesh1 = ic::GeometryGenerator::generate_rectangle_mesh(0.3f, 0.3f);
            mesh1.set_transformation(ic::Mat4x4().set_translation<2>({ -0.35f, 0.0f }));
            
            // Mesh 2
            mesh2 = ic::GeometryGenerator::generate_rectangle_mesh(0.3f, 0.3f);

            shader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex2D, alphaBlendingFragment);
            texture1 = ic::TextureLoader::load_png("resources/textures/discontinuous-square.png");
            
            camera = ic::Camera2D();

            ic::Time::wait(2.5f, [&]() {
                int ringCount = 200;
    
                for (int j = 0; j < ringCount; j++) {
                    int x = rand() % (image.get_width() - 1), y = rand() % (image.get_height() - 1);
                    int ringSize = rand() % 5 + 1;
    
                    image.fill_circle(x, y, ringSize, { 255, 255, 255, (uint8_t) (ic::Noise::perlin_1D(j / (float) ringCount) * 255) });
                }
            });

            
            return true;
        }

        void window_size_changed(int w, int h) override {
            camera.width = w;
            camera.height = h;
        }

        bool handle_event(ic::Event event) override { 
            return true;
        }
    
        bool update() override {
            ic::TextureParameters params; params.magFilter = params.minFilter = ic::TEXTURE_FILTER_LINEAR;
            texture2 = ic::TextureLoader::load(image, params);

            mesh2.set_transformation(ic::Mat4x4().set_translation<2>({ 0.35f, 0.0f }) * ic::Mat4x4().set_scaling<2>({ sin(ic::Time::time), 1.0f }));


            ic::GL::clear_color(ic::Colors::blue);
            
            
            shader.use();
            camera.use(shader);

            texture1.use();
            mesh1.draw(shader);

            texture2.use();
            mesh2.draw(shader);

            return true; 
        }

        void dispose() {
            shader.clear();
            
            texture1.dispose();
            texture2.dispose();

            mesh1.dispose();
            mesh2.dispose();
        }
};

int main() {
    ic::Debug::create_console();

    Transparency2D application;
    
    application.construct(640, 480);
    application.start();

    return 0;
}