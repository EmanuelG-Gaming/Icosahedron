#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/2d/Camera2D.h>
#include <Icosahedron/scene/2d/Mesh2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>


/* Displays a textured rectangle that can be moved around using the WASD keys or the arrow keys. */
class TexturedRectangle : public ic::Application {
    ic::Texture texture;
    ic::Shader shader;
    ic::Camera2D camera;

    ic::Mesh2D shape;
    ic::Vec2f shapePosition;

    public:
        bool init() override {
            displayName = "Textured Rectangle Example";
            
            return true;
        }
        
        bool load() override {
            shape = ic::GeometryGenerator::generate_rectangle_mesh(0.2f, 0.2f);
            
            texture = ic::TextureLoader::load_png("resources/textures/transformations.png");
            shader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex2D, ic::Shaders::meshShaderFrag2D);

            camera = ic::Camera2D();
        
            ic::InputHandler::add_input((new ic::KeyboardController())->with_WASD(), "WASD");
            shapePosition = { 0.0f, 0.0f };

            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override {
            auto *controller = ic::InputHandler::find_keyboard("WASD");
            ic::Vec2i dir = controller->get_direction();

            float speed = 1.0f;
            shapePosition.x() += dir.x() * speed * dt;
            shapePosition.y() += dir.y() * speed * dt;
            
            shape.set_transformation(ic::Mat4x4().set_translation(shapePosition));
            

            clear_color(ic::Colors::blue);
            
            shader.use();
            camera.use(shader);

            texture.use();
            shape.draw(shader);
            
            return true; 
        }

        void dispose() override {
            texture.dispose();
            shader.clear();
            
            shape.dispose();
        }
};

int main() {
    TexturedRectangle application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}