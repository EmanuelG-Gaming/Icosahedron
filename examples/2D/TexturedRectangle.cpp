#include <Icosahedron/Core.h>

/* Displays a textured rectangle that can be moved around using the WASD keys or the arrow keys. */
class TexturedRectangle : public ic::Application {
    ic::Texture *texture;
    ic::Shader *shader;
    ic::Camera2D *camera;

    ic::Mesh2D *shape;
    ic::Vec2f shapePosition;
    
    public:
        bool init() override {
            displayName = "Textured Rectangle Example";
            
            return true;
        }
        
        bool load() override {
            shape = ic::GeometryGenerator::get().generate_rectangle_mesh(0.2f, 0.2f);

            texture = ic::TextureLoader::get().load_png("resources/textures/wood.png");
            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D);

            camera = new ic::Camera2D();
        
            inputHandler.add_input((new ic::KeyboardController())->with_WASD(), "WASD");
            shapePosition = { 0.0f, 0.0f };

            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override {
            auto *controller = inputHandler.find_keyboard("WASD");
            ic::Vec2i dir = controller->get_direction();

            float speed = 1.0f;
            shapePosition.x() += dir.x() * speed * dt;
            shapePosition.y() += dir.y() * speed * dt;
            
            shape->set_transformation(ic::Mat4x4().set_translation(shapePosition));
            

            clear_color(ic::Colors::blue);
            
            shader->use();
            camera->use(shader);

            texture->use();
            shape->draw(shader);
            
            return true; 
        }

        void dispose() override {
            texture->dispose();
            shader->clear();
            
            shape->dispose();
        }
};

int main(int argc, char *argv[]) {
    TexturedRectangle application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}