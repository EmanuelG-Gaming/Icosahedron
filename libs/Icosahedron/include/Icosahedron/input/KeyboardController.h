#ifndef IC_KEYBOARD_CONTROLLER
#define IC_KEYBOARD_CONTROLLER

#include <Icosahedron/input/Input.h>

namespace ic {
    struct KeyboardController : public Input {
        public:
            KeyboardController() {}

            void handle_event(ic::Event event, float dt) override;

        private:
            KeyboardInput key = INVALID_KEY;
    };
}

#endif