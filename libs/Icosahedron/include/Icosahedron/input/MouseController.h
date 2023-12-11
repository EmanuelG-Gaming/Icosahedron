#ifndef IC_MOUSE_CONTROLLER_H
#define IC_MOUSE_CONTROLLER_H

#include <vector>

#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/input/Input.h>


namespace ic {
    struct MouseController : public Input {
        public:
            MouseController();

            void update(float dt) override;
            void handle_event(const ic::Event &event, float dt) override;

            /** @brief Adds an action that is called when the mouse is moved. */
            void add_mouse_moved_action(const std::function<void()> &callback);

            /** @brief Adds an action that is called when the mouse button is released. */
            void add_mouse_up_action(const std::function<void()> &callback);

            /** @brief Adds an action that is called when the mouse button is first pressed. */
            void add_mouse_down_action(const std::function<void()> &callback);

            /** @brief Adds an action that is called when the mouse button is held down. */
            void add_mouse_hold_action(const std::function<void()> &callback);

            /** @brief Adds an action that is called when the mouse wheel is scrolled up. */
            void add_mouse_scroll_up_action(const std::function<void()> &callback);

            /** @brief Adds an action that is called when the mouse wheel is scrolled down. */
            void add_mouse_scroll_down_action(const std::function<void()> &callback);

            ic::Vec2i get_cursor_position();
            ic::Vec2i get_relative_motion();
            float get_wheel_direction();
            bool button_held_down();

        protected:
            ic::Vec2i cursorPosition;
            ic::Vec2i relativeMotion;
            float scrollWheelDirection;

            bool mouseHeldDown;
            
            std::vector<ic::MouseInputAction> inputActions;
    };
}
#endif