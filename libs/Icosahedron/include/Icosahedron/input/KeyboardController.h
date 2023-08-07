#ifndef IC_KEYBOARD_CONTROLLER
#define IC_KEYBOARD_CONTROLLER

#include <functional>
#include <map>

#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/input/Input.h>


namespace ic {
    struct KeyboardController : public Input {
        public:
            ic::Vec2i direction;
            KeyboardController();

            void update(float dt) override;

            void add_action(const std::function<void(ic::KeyboardController*)> &callback, Uint8 location);

            Uint8* get_keyboard_state();
            ic::Vec2i get_direction();

        protected:
            Uint8* keyboardState;
            std::map<Uint8, std::function<void(ic::KeyboardController*)>> inputActions;
    };
}

#endif