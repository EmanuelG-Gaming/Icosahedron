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
    ic::Mesh2D mesh;

    ic::TextureAtlas spritesheet;
    ic::Camera2D camera;

    ic::Shader shader;

    public:
        bool init() override {
            displayName = "Animated sprites";
            
            return true;
        }
        
        bool load() override {
            // Alpha blending
            ic::GLStateHandler::enable_blending(ic::SRC_ALPHA, ic::DEST_ONE_MINUS_SRC_ALPHA);


            // Mesh
            mesh = ic::GeometryGenerator::generate_rectangle_mesh(1.0f, 1.0f);
            
            shader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex2D, alphaBlendingFragment);
            
            int images = 10;
            spritesheet = ic::TextureAtlas(9086, 9086);
            for (int i = 1; i <= images; i++) {
                spritesheet.add_entry("sprite-animation-frame-" + std::to_string(i), "resources/textures/animation/frame-" + std::to_string(i) + ".png");
            }
            
            camera = ic::Camera2D();

            return true;
        }

        void window_size_changed(int w, int h) override {
            camera.width = w;
            camera.height = h;
        }
    
        bool update() override {
            set_texture_dynamic(mesh, (int) (ic::Time::globalTime * 5.0f) % (10 - 1));

            clear_color(ic::Colors::blue);
            
            shader.use();
            //camera.use(shader);

            spritesheet.use();

            int count = 1000;
            float w = 1.0f;
            float h = 1.0f;
            for (int i = count; i >= 0; i--) {
                mesh.set_transformation(ic::Mat4x4().set_scaling<2>(ic::Vec2((i / (float) count) * w, (i / (float) count) * h)));
                mesh.draw(shader);
            }

            return true; 
        }

        void dispose() {
            shader.clear();
            
            spritesheet.dispose();
            mesh.dispose();
        }

        void set_texture_dynamic(ic::Mesh2D &m, int entryAtIndex) {
            auto &entry = spritesheet.get_entry(entryAtIndex);
            m.attribute(2, 2, std::vector<float>({ entry.u, entry.v, entry.u2, entry.v, entry.u2, entry.v2, entry.u, entry.v2 }));
        }
};

int main() {
    AnimatedSprites application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}