#include <Icosahedron/input/WASDController.h>

using namespace ic;

WASDController::WASDController() {
    keyboardState = get_keyboard_state();
}

void ic::WASDController::update(float dt) {
    direction = { 0, 0 };

    if (keyboardState[KEY_A] || keyboardState[KEY_LEFT]) {
        direction.x() = -1;
    }
    if (keyboardState[KEY_D] || keyboardState[KEY_RIGHT]) {
        direction.x() = 1;
    }

    if (keyboardState[KEY_S] || keyboardState[KEY_DOWN]) {
        direction.y() = -1;
    }
    if (keyboardState[KEY_W] || keyboardState[KEY_UP]) {
        direction.y() = 1;
    }


    for (auto pair : inputActions) {
        const Uint8 keyCode = pair.first;
        auto call = pair.second;

        if (keyboardState[keyCode]) {
            call(this);
        }
    }
}

void ic::WASDController::add_action(const std::function<void(ic::WASDController*)> &callback, Uint8 location) {
    inputActions[location] = callback;
}


Uint8* ic::WASDController::get_keyboard_state() {
     return const_cast<Uint8*>(SDL_GetKeyboardState(NULL));
}