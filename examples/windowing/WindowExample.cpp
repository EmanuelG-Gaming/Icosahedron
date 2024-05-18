#include <Icosahedron/Application.h>
#include <Icosahedron/graphics/Colors.h>


// Shows an empty blue window.
class WindowExample : public ic::Application {
    public:
        bool load() override {
            // This function is called after init() and after setting up the window

            return true;
        }

        void window_size_changed(int w, int h) override {
            // Called when the window is resized, and also if it is changed to fullscreen mode
            
        }

        bool handle_event(ic::Event event) override { 
            // Called when events are retrieved and polled 

            return true;
        }

        bool update() override {
            // This is called once every frame
        
            ic::GL::clear_color(ic::Colors::blue);
            return true; 
        }

        void dispose() override {
            // Occurs when the application has to close

        }
};

// Short version:
/*
#include <Icosahedron/Application.h>
#include <Icosahedron/graphics/Colors.h>

class WindowExample : public ic::Application {
    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }

        bool update(float dt) override {
            clear_color(ic::Colors::blue);
            return true; 
        }
};
*/

int main() {
    ic::Engine engine;
    engine.window.create_GL_context();

    

    application.construct("Example window", 640, 480);
    application.start();

    return 0;
}