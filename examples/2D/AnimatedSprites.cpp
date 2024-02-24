#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/gl/TextureAtlas.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/Image.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>
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

class AnimatedSprites : public ic::Application {
    ic::Image image;
    ic::Mesh2D mesh1, mesh2;

    ic::Texture texture1, texture2;
    ic::TextureAtlas spritesheet;
    ic::Camera2D camera;

    ic::Shader shader;
    float time;

    public:
        bool init() override {
            displayName = "Animated sprites";
            
            return true;
        }
        
        bool load() override {
            // Alpha blending
            ic::GLStateHandler::enable_blending(ic::SRC_ALPHA, ic::DEST_ONE_MINUS_SRC_ALPHA);

            image = ic::Image(64, 64);
            int rings = 10;
            int ringSize = 2;
            int ringCount = 10;

            for (int j = 0; j < ringCount; j++) {
                int x = rand() % (image.get_width() - 1), y = rand() % (image.get_height() - 1);

                for (int i = 0; i < 10; i++) {
                    image.fill_circle(x, y, (rings + 1) * ringSize - i, { 255, 255, 255, (uint8_t) (255 - i * rings * 2.5f) });
                }
            }

            // Mesh 1
            mesh1 = ic::GeometryGenerator::generate_rectangle_mesh(0.3f, 0.3f);
            mesh1.set_transformation(ic::Mat4x4().set_translation<2>({ -0.35f, 0.0f }));
            
            // Mesh 2
            mesh2 = ic::GeometryGenerator::generate_rectangle_mesh(0.3f, 0.3f);
            mesh2.set_transformation(ic::Mat4x4().set_translation<2>({ 0.35f, 0.0f }));
            

            shader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex2D, alphaBlendingFragment);
            texture1 = ic::TextureLoader::load_png("resources/textures/discontinuous-square.png");
            texture2 = ic::TextureLoader::load(image);

            int images = 3;
            spritesheet = ic::TextureAtlas(32 * images, 32);
            for (int i = 1; i <= 3; i++) {
                spritesheet.add_entry("sprite-animation-frame-" + std::to_string(i), "resources/textures/animation/frame-" + std::to_string(i) + ".png");
            }
            
            camera = ic::Camera2D();

            time = 0.0f;

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

            clear_color(ic::Colors::blue);
            
            
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

        void set_texture_dynamic(ic::Mesh2D &mesh, int entryAtIndex) {
            auto &entry = spritesheet.get_entry(entryAtIndex);
            
        }
};

int main() {
    AnimatedSprites application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}