/*
#include <Icosahedron/Application.h>
#include <Icosahedron/graphics/Colors.h>


// Shows an empty blue window.
class WindowExample : public ic::Application {
    public:
        bool init() override {
            this->window.set_title("Example window");
            
            return true;
        }
        
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
*/

// Short version:
/*
#include <Icosahedron/Application.h>
#include <Icosahedron/graphics/Colors.h>

class WindowExample : public ic::Application {
    public:
        bool init() override {
            this->window.set_title("Example window");
            return true;
        }

        bool update() override {
            ic::GL::clear_color(ic::Colors::blue);
            return true;
        }
};
*/

/** A shorter main function declaration. You can still use argc 
 *  and argv as parameters, though. */
/*
int main() {
    WindowExample application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/



#include <Icosahedron/Application.h>
#include <IcosahedronDebug/ConsoleOutput.h>

ic::Engine engine;

void start_application() {
    engine.window.set(1028, 304, "test window");
    engine.construct();

    bool enabled = true;
    ic::Event event;
    while (enabled) {
        while (engine.poll_events(event)) {
            enabled = engine.process_window_callbacks(event);
        }

        ic::GL::clear_color(ic::Colors::blue);

        engine.window.swap_buffers();
	    engine.tick();
    }
    
    engine.close();
}

int main() {
    ic::Debug::create_console();
    start_application();

    return 0;
}