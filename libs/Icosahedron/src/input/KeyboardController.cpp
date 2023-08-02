#include <Icosahedron/input/KeyboardController.h>

void ic::KeyboardController::handle_event(ic::Event event, float dt) {
    keyboardState = get_keyboard_state();
}

void ic::KeyboardController::update(float dt) {
    direction = { 0, 0 };

    if (keyboardState[KEY_A] || keyboardState[KEY_LEFT]) {
        direction[0] = -1;
    }
    if (keyboardState[KEY_D] || keyboardState[KEY_RIGHT]) {
        direction[0] = 1;
    }

    if (keyboardState[KEY_S] || keyboardState[KEY_DOWN]) {
        direction[1] = -1;
    }
    if (keyboardState[KEY_W] || keyboardState[KEY_UP]) {
        direction[1] = 1;
    }


    for (auto pair : inputActions) {
        const Uint8 keyCode = pair.first;
        auto call = pair.second;

        if (keyboardState[keyCode]) {
            call();
        }
    }
}

void ic::KeyboardController::add_action(const std::function<void()> &callback, Uint8 location) {
    inputActions[location] = callback;
}


Uint8* ic::KeyboardController::get_keyboard_state() {
     return const_cast<Uint8*>(SDL_GetKeyboardState(NULL));
}

ic::Vector<int, 2> ic::KeyboardController::get_direction() {
     return direction;
}