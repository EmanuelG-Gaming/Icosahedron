#ifndef IC_INPUT_H
#define IC_INPUT_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>

using INPUT_LOCATION = std::string;

enum KeyboardInput {
    INVALID_KEY = 0,
    
    KEY_W = SDL_SCANCODE_W,
    KEY_A = SDL_SCANCODE_A,
    KEY_S = SDL_SCANCODE_S,
    KEY_D = SDL_SCANCODE_D,

    KEY_UP = SDL_SCANCODE_UP,
    KEY_LEFT = SDL_SCANCODE_LEFT,
    KEY_DOWN = SDL_SCANCODE_DOWN,
    KEY_RIGHT = SDL_SCANCODE_RIGHT,

    KEY_SPACE = SDL_SCANCODE_SPACE,
};

namespace ic {
    using Event = SDL_Event;
    using KeyboardEvent = SDL_KeyboardEvent;
    using Keycode = SDL_Keycode;

    struct Input {
        public:
            virtual void handle_event(ic::Event event, float dt) {}
            virtual void update(float dt) {}
    };
}

#endif