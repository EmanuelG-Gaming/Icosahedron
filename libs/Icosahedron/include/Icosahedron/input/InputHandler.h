#ifndef IC_INPUT_HANDLER_H
#define IC_INPUT_HANDLER_H

#include <map>

#include <Icosahedron/input/Input.h>
#include <Icosahedron/input/KeyboardController.h>
#include <Icosahedron/input/MouseController.h>

namespace ic {
    namespace InputHandler {
        void handle(ic::Event event, float dt);
        void update(float dt);
        
        void add_input(ic::Input* input, const INPUT_LOCATION &location);
        ic::Input *find_input(const INPUT_LOCATION &location);

        ic::KeyboardController *find_keyboard(const INPUT_LOCATION &location);
        ic::MouseController *find_mouse(const INPUT_LOCATION &location);

        namespace {
            std::map<INPUT_LOCATION, ic::Input*> inputs;
        };
    };
}
#endif