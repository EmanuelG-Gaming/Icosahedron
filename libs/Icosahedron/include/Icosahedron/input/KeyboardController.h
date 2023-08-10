#ifndef IC_KEYBOARD_CONTROLLER_H
#define IC_KEYBOARD_CONTROLLER_H

#include <map>

#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/input/Input.h>


namespace ic {
    struct KeyboardController : public Input {
        public:
            ic::Vec2i direction;
            KeyboardController();

            void update(float dt) override;
            void handle_event(ic::Event event, float dt) override;

            /* Adds an action that is called rapidly when a key is pressed. */
            void add_action(const std::function<void()> &callback, Uint8 location);

            /* Adds an action that is called when a key is released. */
            void add_key_up_action(const std::function<void()> &callback, Uint8 location);

            /* Adds an action that is called when a key is first down. */
            void add_key_down_action(const std::function<void()> &callback, Uint8 location);

            /* Adds WASD and arrow bindings to this keyboard handler. 
             * Returns the controller, for chaining operations. */
            ic::KeyboardController *with_WASD();

            Uint8* get_keyboard_state();
            ic::Vec2i get_direction();

        protected:
            Uint8* keyboardState;
            std::map<Uint8, ic::KeyboardInputAction> inputActions;
    };
}

#endif