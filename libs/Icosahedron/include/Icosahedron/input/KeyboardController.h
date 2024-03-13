#ifndef IC_KEYBOARD_CONTROLLER_H
#define IC_KEYBOARD_CONTROLLER_H

#include <map>

#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/input/Input.h>


namespace ic {
    struct KeyboardController : ic::Input {
        public:
            ic::Vec2i direction;
            
            KeyboardController();

            void update(float dt) override;
            void handle_event(const ic::Event &event, float dt) override;

            /** @brief Adds an action that is called rapidly when a key is pressed. */
            void add_action(const std::function<void()> &callback, Uint8 location);

            /** @brief Adds an action that is called when a key is released. */
            void add_key_up_action(const std::function<void()> &callback, Uint8 location);

            /** @brief Adds an action that is called when a key is first down. */
            void add_key_down_action(const std::function<void()> &callback, Uint8 location);

            /** @brief Adds WASD and arrow bindings to this keyboard handler. 
             *  @returns The controller, for chaining operations. */
            ic::KeyboardController *with_WASD();

            Uint8* get_keyboard_state();
            Uint8 &is_key_pressed(KeyboardInput input);
            ic::Vec2i get_direction();
            bool key_pressed();

        protected:
            Uint8* keyboardState;
            std::map<Uint8, ic::KeyboardInputAction> inputActions;
    };
}

#endif