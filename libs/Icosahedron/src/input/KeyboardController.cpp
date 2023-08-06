#include <Icosahedron/input/KeyboardController.h>

using namespace ic;

KeyboardController::KeyboardController() {
    keyboardState = get_keyboard_state();
}

void ic::KeyboardController::update(float dt) {
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

ic::Vec2i ic::KeyboardController::get_direction() {
     return direction;
}