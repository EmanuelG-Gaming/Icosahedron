#ifndef IC_INPUT_HANDLER_H
#define IC_INPUT_HANDLER_H

#include <map>

#include <Icosahedron/input/Input.h>
#include <Icosahedron/input/KeyboardController.h>
#include <Icosahedron/input/MouseController.h>

namespace ic {
    namespace InputHandler {
        void handle(ic::Event event);
        void update();
        
        void add_input(ic::Input* input, const INPUT_LOCATION &location);
        ic::Input *find_input(const INPUT_LOCATION &location);

        ic::KeyboardController *find_keyboard(const INPUT_LOCATION &location);
        ic::MouseController *find_mouse(const INPUT_LOCATION &location);

        //// Keyboard
        //
        //void add_keyboard_hold_action(const std::function<void()> &callback, Uint8 location);
        //void add_keyboard_up_action(const std::function<void()> &callback, Uint8 location);
        //void add_keyboard_down_action(const std::function<void()> &callback, Uint8 location);
//
//
        //// Mouse
//
        //void add_mouse_moved_action(const std::function<void()> &callback);
//
        //void add_mouse_hold_action(const std::function<void()> &callback);
        //void add_mouse_up_action(const std::function<void()> &callback);
        //void add_mouse_down_action(const std::function<void()> &callback);
        //
        //void add_mouse_scroll_up_action(const std::function<void()> &callback);
        //void add_mouse_scroll_down_action(const std::function<void()> &callback);
//
//
        //ic::KeyboardController *get_keyboard();
        //ic::MouseController *get_mouse();

        namespace {
            std::map<INPUT_LOCATION, ic::Input*> inputs;
            //ic::KeyboardController keyboard;
            //ic::MouseController mouse;
        };
    };
}
#endif