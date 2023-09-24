#ifndef IC_EXAMPLE_TEXTURED_RECTANGLE_H
#define IC_EXAMPLE_TEXTURED_RECTANGLE_H

/* Displays a textured rectangle that can be moved around using the WASD keys or the arrow keys. */
#include <Icosahedron/Core.h>

class TexturedRectangle : public ic::Application {
    ic::Texture<ic::T2D> *texture;
    ic::Shader *shader;
    ic::Camera2D *camera;

    ic::Mesh2D *shape;
    ic::Vec2f shapePosition;
    
    public:
        bool init() override {
            displayName = "Example window";
            
            return true;
        }
        
        bool load() override {
            shape = new ic::Mesh2D(ic::GeometryGenerator::get().generate_rectangle(0.2f, 0.2f));

            shape->jump_attribute();
            shape->add_attribute("textureCoords", 2, ic::GeometryGenerator::get().generate_UV_rectangle());
            shape->set_index_buffer({ 0, 1, 2, 0, 2, 3 });
            shape->set_material(ic::MeshMaterial2D(ic::Colors::white, 1.0f));

            texture = new ic::Texture<ic::T2D>("resources/textures/wood.png");
            shader = new ic::Shader(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D, false);

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

#endif