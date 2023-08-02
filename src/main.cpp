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


/* Example2: Displays a green rectangle that can be moved around using the WASD keys or the arrow keys. */
#include <Icosahedron/Core.h>

class Example2 : public ic::Application {
    ic::Batch2D *batch;
    ic::RectangleShape *shape;

    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }
        
        bool load() override {
            batch = new ic::Batch2D(1000, TRIANGLES);
            shape = new ic::RectangleShape();

            inputHandler.add_input(new ic::KeyboardController(), "WASD");

            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override { 
            float speed = 1.0f;
            ic::KeyboardController *controller = (ic::KeyboardController*) inputHandler.find_input("WASD");
            ic::Vector<int, 2> dir = controller->get_direction();

            shape->position[0] += dir[0] * speed * dt;
            shape->position[1] += dir[1] * speed * dt;


            clear_color(ic::Colors::blue);

            shape->draw(batch, ic::Colors::green);
            batch->render();

            return true; 
        }

        void dispose() override {
            batch->dispose();
        }
};

int main(int argc, char *argv[]) {
    Example2 application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}