#ifndef IC_INPUT_H
#define IC_INPUT_H

#include <string>

#include <SDL2/SDL.h>


using INPUT_LOCATION = std::string;

enum KeyboardInput {
    INVALID_KEY = 0,
    
    KEY_W = SDLK_w,
    KEY_A = SDLK_a,
    KEY_S = SDLK_s,
    KEY_D = SDLK_d,

    KEY_SPACE = SDLK_SPACE,
};

namespace ic {
    using Event = SDL_Event;

    struct Input {
        public:
            virtual void handle_event(ic::Event event, float dt) = 0;
    };
}

#endif