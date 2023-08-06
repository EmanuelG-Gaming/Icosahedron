// Examples (uncomment any of these)
/* Example 1: Displays a blue window on startup. */
/*
#include <Icosahedron/Core.h>

class Example1 : public ic::Application {
    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }
        
        bool load() override {
            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override { 
            clear_color(ic::Colors::blue);
            return true; 
        }
};

int main(int argc, char *argv[]) {
    Example1 application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/


/* Example2: Displays a textured rectangle that can be moved around using the WASD keys or the arrow keys. */
/*
#include <Icosahedron/Core.h>

class Example2 : public ic::Application {
    ic::Batch2D *batch;
    ic::Texture<ic::T2D> *texture;
    ic::Shader *shader;

    ic::RectangleShape *shape;
    
    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }
        
        bool load() override {
            batch = new ic::Batch2D(1000, ic::TRIANGLES);
            shape = new ic::RectangleShape();
            texture = new ic::Texture<ic::T2D>({"resources/textures/wood.png"});
            shader = shaders.basicTextureShader2D;

            inputHandler.add_input(new ic::WASDController(), "WASD");

            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override { 
            float speed = 1.0f;
            ic::KeyboardController *controller = (ic::KeyboardController*) inputHandler.find_input("WASD");
            ic::Vec2i dir = controller->get_direction();

            shape->r.position.x() += dir.x() * speed * dt;
            shape->r.position.y() += dir.y() * speed * dt;


            clear_color(ic::Colors::blue);

            shader->use();
            texture->use();
            shape->draw(batch, ic::Colors::white);
            batch->render();

            return true; 
        }

        void dispose() override {
            batch->dispose();
            texture->dispose();
            shader->clear();
        }
};

int main(int argc, char *argv[]) {
    Example2 application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/

/* Example3: A recreation of pong. */
#include <Icosahedron/Core.h>

class Example3 : public ic::Application {
    ic::Batch2D *batch;
    ic::Texture<ic::T2D> *texture;
    ic::Shader *shader;

    ic::RectangleShape *shape;
    
    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }
        
        bool load() override {
            batch = new ic::Batch2D(1000, ic::TRIANGLES);
            shape = new ic::RectangleShape();
            texture = new ic::Texture<ic::T2D>({"resources/textures/wood.png"});
            shader = shaders.basicTextureShader2D;

            inputHandler.add_input(new ic::WASDController(), "WASD");

            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override { 
            float speed = 1.0f;
            ic::KeyboardController *controller = (ic::KeyboardController*) inputHandler.find_input("WASD");
            ic::Vec2i dir = controller->get_direction();

            shape->r.position.x() += dir.x() * speed * dt;
            shape->r.position.y() += dir.y() * speed * dt;


            clear_color(ic::Colors::black);

            shader->use();
            texture->use();
            shape->draw(batch, ic::Colors::white);
            batch->render();

            return true; 
        }

        void dispose() override {
            batch->dispose();
            texture->dispose();
            shader->clear();
        }
};

int main(int argc, char *argv[]) {
    Example3 application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}