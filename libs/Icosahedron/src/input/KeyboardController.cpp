#include <Icosahedron/input/KeyboardController.h>

using namespace ic;

KeyboardController::KeyboardController() {
    keyboardState = get_keyboard_state();
}

void ic::KeyboardController::update(float dt) {
    direction = { 0, 0 };

    for (auto pair : inputActions) {
        auto act = pair.second;
        if (act.type != KB_INPUT_HOLD) continue;

        const Uint8 keyCode = pair.first;
        if (keyboardState[keyCode]) {
            act.callback();
        }
    }
}
void ic::KeyboardController::handle_event(ic::Event event, float dt) {
    ic::Event *e = &event;

    if (event.type == SDL_KEYDOWN) {
        for (auto pair : inputActions) {
            auto act = pair.second;
            if (act.type != KB_INPUT_DOWN) continue;

            const Uint8 keyCode = pair.first;
            if (e->key.keysym.scancode == keyCode) {
                act.callback();
            }
        }
    }

    if (event.type == SDL_KEYUP) {
        for (auto pair : inputActions) {
            auto act = pair.second;
            if (act.type != KB_INPUT_UP) continue;

            const Uint8 keyCode = pair.first;
            if (e->key.keysym.scancode == keyCode) {
                act.callback();
            }
        }
    }
}

void ic::KeyboardController::add_action(const std::function<void()> &callback, Uint8 location) {
    inputActions[location] = { KB_INPUT_HOLD, callback };
}

void ic::KeyboardController::add_key_up_action(const std::function<void()> &callback, Uint8 location) {
    inputActions[location] = { KB_INPUT_UP, callback };
}

void ic::KeyboardController::add_key_down_action(const std::function<void()> &callback, Uint8 location) {
    inputActions[location] = { KB_INPUT_DOWN, callback };
}

ic::KeyboardController *ic::KeyboardController::with_WASD() {
    add_action([this]() {this->direction.x() = 1;}, KEY_D);
    add_action([this]() {this->direction.x() = -1;}, KEY_A);
    add_action([this]() {this->direction.y() = 1;}, KEY_W);
    add_action([this]() {this->direction.y() = -1;}, KEY_S);

    add_action([this]() {this->direction.x() = 1;}, KEY_RIGHT);
    add_action([this]() {this->direction.x() = -1;}, KEY_LEFT);
    add_action([this]() {this->direction.y() = 1;}, KEY_UP);
    add_action([this]() {this->direction.y() = -1;}, KEY_DOWN);

    return this;
}


Uint8* ic::KeyboardController::get_keyboard_state() {
    return const_cast<Uint8*>(SDL_GetKeyboardState(NULL));
}

ic::Vec2i ic::KeyboardController::get_direction() {
    return direction;
}