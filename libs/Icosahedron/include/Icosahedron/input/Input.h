#ifndef IC_INPUT_H
#define IC_INPUT_H

#include <string>
#include <functional>

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL.h>


using INPUT_LOCATION = std::string;

enum KeyboardInput {
    INVALID_KEY = -1,
    
    
    KEY_Q = SDL_SCANCODE_Q,
    KEY_W = SDL_SCANCODE_W,
    KEY_E = SDL_SCANCODE_E,
    KEY_R = SDL_SCANCODE_R,
    KEY_T = SDL_SCANCODE_T,
    KEY_Y = SDL_SCANCODE_Y,
    KEY_U = SDL_SCANCODE_U,
    KEY_I = SDL_SCANCODE_I,
    KEY_O = SDL_SCANCODE_O,
    KEY_P = SDL_SCANCODE_P,

    KEY_A = SDL_SCANCODE_A,
    KEY_S = SDL_SCANCODE_S,
    KEY_D = SDL_SCANCODE_D,

    KEY_F = SDL_SCANCODE_F,
    KEY_G = SDL_SCANCODE_G,
    KEY_H = SDL_SCANCODE_H,
    KEY_J = SDL_SCANCODE_J,
    KEY_K = SDL_SCANCODE_K,
    KEY_L = SDL_SCANCODE_L,
    KEY_Z = SDL_SCANCODE_Z,
    KEY_X = SDL_SCANCODE_X,
    KEY_C = SDL_SCANCODE_C,
    KEY_V = SDL_SCANCODE_V,
    KEY_B = SDL_SCANCODE_B,
    KEY_N = SDL_SCANCODE_N,
    KEY_M = SDL_SCANCODE_M,

    KEY_F1 = SDL_SCANCODE_F1,
    KEY_F2 = SDL_SCANCODE_F2,
    KEY_F3 = SDL_SCANCODE_F3,
    KEY_F4 = SDL_SCANCODE_F4,
    KEY_F5 = SDL_SCANCODE_F5,
    KEY_F6 = SDL_SCANCODE_F6,
    KEY_F7 = SDL_SCANCODE_F7,
    KEY_F8 = SDL_SCANCODE_F8,
    KEY_F9 = SDL_SCANCODE_F9,
    KEY_F10 = SDL_SCANCODE_F10,
    KEY_F11 = SDL_SCANCODE_F11,
    KEY_F12 = SDL_SCANCODE_F12,

    KEY_UP = SDL_SCANCODE_UP,
    KEY_LEFT = SDL_SCANCODE_LEFT,
    KEY_DOWN = SDL_SCANCODE_DOWN,
    KEY_RIGHT = SDL_SCANCODE_RIGHT,

    KEY_SPACE = SDL_SCANCODE_SPACE,
};

enum KeyboardInputActionTypes {
    INVALID_KB_INPUT_ACTION = -1,

    KB_INPUT_UP = 0,
    KB_INPUT_DOWN = 1,

    KB_INPUT_HOLD = 2,
};

namespace ic {
    using Event = SDL_Event;
    using KeyboardEvent = SDL_KeyboardEvent;
    using Keycode = SDL_Keycode;

    struct KeyboardInputAction {
        KeyboardInputActionTypes type;
        std::function<void()> callback;
    };

    struct Input {
        public:
            virtual void handle_event(ic::Event event, float dt) {}
            virtual void update(float dt) {}
    };
}

#endif