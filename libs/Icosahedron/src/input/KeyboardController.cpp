#include <Icosahedron/input/KeyboardController.h>

using namespace ic;

KeyboardController::KeyboardController() {
    keyboardState = get_keyboard_state();
}

void ic::KeyboardController::update(float dt) {
    direction = { 0, 0 };

    for (auto &pair : inputActions) {
        auto &act = pair.second;

        if (keyboardState[pair.first]) {
            if (act.held != nullptr) {
                act.held();
            }

            //std::cout << "key hold detected" << "\n";
        }
    }
}


void ic::KeyboardController::handle_event(const ic::Event &event, float dt) {
    if (event.type == SDL_KEYDOWN) {
        for (auto &pair : inputActions) {
            auto &act = pair.second;

            if (event.key.keysym.scancode == pair.first && !act.keyPressed) {
                act.keyPressed = true;

                if (act.pressed != nullptr) {
                    act.pressed();
                }

                //std::cout << "key press detected" << "\n";
            }
        }
    }

    if (event.type == SDL_KEYUP) {
        for (auto &pair : inputActions) {
            auto &act = pair.second;
            
            if (event.key.keysym.scancode == pair.first) {
                act.keyPressed = false;
                
                if (act.released != nullptr) {
                    act.released();
                }

                //std::cout << "key release detected" << "\n";
            }
        }
    }
}

void ic::KeyboardController::add_action(const std::function<void()> &callback, Uint8 location) {
    if (inputActions.find(location) == inputActions.end()) {
        ic::KeyboardInputAction action = ic::KeyboardInputAction();
        action.held = callback;

        inputActions[location] = action;
        return;
    }

    inputActions[location].held = callback;
}

void ic::KeyboardController::add_key_up_action(const std::function<void()> &callback, Uint8 location) {
    if (inputActions.find(location) == inputActions.end()) {
        ic::KeyboardInputAction action = ic::KeyboardInputAction();
        action.released = callback;

        inputActions[location] = action;
        return;
    }

    inputActions[location].released = callback;
}

void ic::KeyboardController::add_key_down_action(const std::function<void()> &callback, Uint8 location) {
    if (inputActions.find(location) == inputActions.end()) {
        ic::KeyboardInputAction action = ic::KeyboardInputAction();
        action.pressed = callback;

        inputActions[location] = action;
        return;
    }

    inputActions[location].pressed = callback;
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

Uint8 &ic::KeyboardController::is_key_pressed(KeyboardInput input) {
    return keyboardState[input];
}

bool ic::KeyboardController::key_pressed() {
    //return keyIsPressed;
    return false;
}