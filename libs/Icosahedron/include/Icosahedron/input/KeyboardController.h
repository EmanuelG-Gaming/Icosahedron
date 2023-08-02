#ifndef IC_KEYBOARD_CONTROLLER
#define IC_KEYBOARD_CONTROLLER

#include <functional>
#include <map>

#include <Icosahedron/math/geom/Vector.h>

#include <Icosahedron/input/Input.h>


namespace ic {
    /* An input that uses the WASD keys to get a direction vector.
     * Also allows for more keyboard actions to be added. */
    struct KeyboardController : public Input {
        public:
            KeyboardController() {}

            void handle_event(ic::Event event, float dt) override;
            void update(float dt) override;

            void add_action(const std::function<void()> &callback, Uint8 location);

            Uint8* get_keyboard_state();
            ic::Vector<int, 2> get_direction();

        private:
            Uint8* keyboardState;
            ic::Vector<int, 2> direction;
            std::map<Uint8, std::function<void()>> inputActions;
    };
}

#endif