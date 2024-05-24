#include <Icosahedron/Application.h>
#include <Icosahedron/graphics/Colors.h>

int main() {

    ic::Engine engine;

    engine.construct_window();
    engine.create_GL_context();
    
    engine.window.set("Example window", 640, 480);
    engine.window.set_vsync(1);

    bool enabled = true;
    ic::Event event;

    while (enabled) {
        while (engine.poll_events(event)) {
            if (!engine.process_window_callbacks(event)) {
                enabled = false;
                break;
            }
        }
        ic::InputHandler::update();

        // Updating every frame
        ic::GL::clear_color(ic::Colors::green);
        // --------------------

        engine.window.swap_buffers();
	    engine.tick();
    }

    return 0;
}
