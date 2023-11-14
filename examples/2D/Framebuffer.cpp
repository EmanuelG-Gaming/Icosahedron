#include <Icosahedron/Application.h>

#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Framebuffer.h>
#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/util/GeometryGenerator.h>
#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>


std::string screenVertex = IC_ADD_GLSL_DEFINITION(
    layout (location = 0) in vec2 position;
    layout (location = 2) in vec2 tCoords;

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

    uniform sampler2D screenTexture;

    out vec4 outColor;

    void main() {
        vec3 color = texture(screenTexture, vTCoords).rgb;
        
        outColor = vec4(color, 1.0);
    }
);

const std::size_t MESH_COUNT = 128;

/** Uses a framebuffer with a lower resolution to achieve a "pixelation" effect. */
class Framebuffer : public ic::Application {
    ic::Mesh2D *screenQuad;
    ic::Mesh2D* meshes[MESH_COUNT];

    ic::Framebuffer *postProcessing;

    ic::Texture *texture;
    ic::Camera2D *camera;

    std::size_t PIXEL_DENOMINATOR;

    ic::Shader *shader, *screenShader;
    float time;

    public:
        bool init() override {
            displayName = "Framebuffers";
            scaling = ic::WindowScaling::resizeable;

            return true;
        }
        
        bool load() override {
            for (int i = 0; i < MESH_COUNT; i++) {
                float x = (rand() % 100 / 100.0f - 0.5f) * 3.0f;
                float y = (rand() % 100 / 100.0f - 0.5f) * 3.0f;
                
                ic::Mesh2D *mesh = ic::GeometryGenerator::get().generate_regular_polygon_mesh(3, 0.3f);
                mesh->add_attribute(1, 3, { ic::Colors::black, ic::Colors::black, ic::Colors::white });
    
                mesh->set_material(ic::MeshMaterial2D(ic::Colors::white, 0.0f));
                mesh->set_transformation(ic::Mat4x4().set_translation<2>({ x, y }));

                meshes[i] = mesh;
            }

            screenQuad = ic::GeometryGenerator::get().generate_rectangle_mesh(1.0f, 1.0f);

            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D);
            screenShader = ic::ShaderLoader::get().load(screenVertex, screenFragment);
            texture = ic::TextureLoader::get().load_png("resources/textures/wood.png");
            
            PIXEL_DENOMINATOR = 2;
            postProcessing = new ic::Framebuffer(ic::TEXTURE_ATTACH_COLOR_0, ic::TEXTURE_RGBA, IC_WINDOW_WIDTH / PIXEL_DENOMINATOR, IC_WINDOW_HEIGHT / PIXEL_DENOMINATOR);

            camera = new ic::Camera2D();


            ic::MouseController *controller = new ic::MouseController();
            controller->add_mouse_scroll_up_action([this]() { 
                float p = ic::InputHandler::get().find_mouse("mouse")->get_wheel_direction() * 0.5f;
                PIXEL_DENOMINATOR = (std::size_t) std::max(1.0f, std::min(PIXEL_DENOMINATOR + p, 32.0f));

                window_size_changed(IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            });
            controller->add_mouse_scroll_down_action([this]() { 
                float p = ic::InputHandler::get().find_mouse("mouse")->get_wheel_direction() * 0.5f;
                PIXEL_DENOMINATOR = (std::size_t) std::max(1.0f, std::min(PIXEL_DENOMINATOR + p, 32.0f));

                window_size_changed(IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            });

            ic::InputHandler::get().add_input(controller, "mouse");

            ic::InputHandler::get().add_input((new ic::KeyboardController())->with_WASD(), "WASD");
            
            time = 0.0f;
            
            return true;
        }

        void window_size_changed(int w, int h) override {
            camera->width = w;
            camera->height = h;

            postProcessing->resize(w / PIXEL_DENOMINATOR, h / PIXEL_DENOMINATOR);
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            time += dt;

            auto *controller = ic::InputHandler::get().find_keyboard("WASD");
            ic::Vec2i dir = controller->get_direction();

            float speed = 1.5f;
            camera->position.x() += dir.x() * speed * dt;
            camera->position.y() += dir.y() * speed * dt;
            
            postProcessing->use();
            clear_color(ic::Colors::blue);
            
            states.set_viewport(IC_WINDOW_WIDTH / PIXEL_DENOMINATOR, IC_WINDOW_HEIGHT / PIXEL_DENOMINATOR);

            shader->use();
            camera->use(shader);

            
            texture->use();
            for (int i = 0; i < MESH_COUNT; i++) {
                meshes[i]->draw(shader);
            }
            texture->unuse();

            postProcessing->unuse();


            // Rendering to the screen quad
            clear_color(ic::Colors::cyan);
            states.set_viewport(IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);

            screenShader->use();
            postProcessing->use_texture();

            screenQuad->draw(screenShader);

            return true; 
        }

        void dispose() {
            shader->clear();
            screenShader->clear();

            texture->dispose();
            
            for (int i = 0; i < MESH_COUNT; i++) {
                meshes[i]->dispose();
            }
            screenQuad->dispose();
        }
};

int main(int argc, char *argv[]) {
    Framebuffer application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}