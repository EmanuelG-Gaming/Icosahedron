#include <Icosahedron/input/MouseController.h>

using namespace ic;

MouseController::MouseController() {
    this->mouseHeldDown = false;
}

void ic::MouseController::update(float dt) {
    if (this->mouseHeldDown) {
        for (auto &action : inputActions) {
            if (action.type != MOUSE_INPUT_HOLD) continue;

            action.callback();
        }
    }
}

void ic::MouseController::handle_event(const ic::Event &event, float dt) {
    int x = 0, y = 0;
    Uint32 state = SDL_GetMouseState(&x, &y);

    this->cursorPosition = { x, y };
    this->relativeMotion = { 0, 0 };
    this->scrollWheelDirection = 0.0f;

    if (event.type == SDL_MOUSEMOTION) {
        this->relativeMotion = { event.motion.xrel, event.motion.yrel };
    }
    if (event.type == SDL_MOUSEWHEEL) {
        this->scrollWheelDirection = event.wheel.y;
    }


    // Events
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        this->mouseHeldDown = true;
        std::cout << "mouse down" << "\n";

        for (auto &action : inputActions) {
            if (action.type != MOUSE_INPUT_DOWN) continue;

            action.callback();
        }
    }

    if (event.type == SDL_MOUSEBUTTONUP) {
        this->mouseHeldDown = false;
        std::cout << "mouse up" << "\n";
        
        for (auto &action : inputActions) {
            if (action.type != MOUSE_INPUT_UP) continue;

            action.callback();
        }
    }

    if (event.type == SDL_MOUSEWHEEL) {
        if (event.wheel.y > 0) { // Scroll up
            for (auto &action : inputActions) {
                if (action.type != MOUSE_INPUT_SCROLL_UP) continue;
    
                action.callback();
            }
        } else if (event.wheel.y < 0) { // Scroll down
            for (auto &action : inputActions) {
                if (action.type != MOUSE_INPUT_SCROLL_DOWN) continue;
    
                action.callback();
            }
        }
    }

    if (event.type == SDL_MOUSEMOTION) {
        for (auto &action : inputActions) {
            if (action.type != MOUSE_INPUT_MOVED) continue;

            action.callback();
        }
    }
}

void ic::MouseController::add_mouse_moved_action(const std::function<void()> &callback) {
    inputActions.push_back({ MOUSE_INPUT_MOVED, callback });
}

void ic::MouseController::add_mouse_up_action(const std::function<void()> &callback) {
    inputActions.push_back({ MOUSE_INPUT_UP, callback });
}

void ic::MouseController::add_mouse_down_action(const std::function<void()> &callback) {
    inputActions.push_back({ MOUSE_INPUT_DOWN, callback });
}

void ic::MouseController::add_mouse_hold_action(const std::function<void()> &callback) {
    inputActions.push_back({ MOUSE_INPUT_HOLD, callback });
}


void ic::MouseController::add_mouse_scroll_up_action(const std::function<void()> &callback) {
    inputActions.push_back({ MOUSE_INPUT_SCROLL_UP, callback });
}

void ic::MouseController::add_mouse_scroll_down_action(const std::function<void()> &callback) {
    inputActions.push_back({ MOUSE_INPUT_SCROLL_DOWN, callback });
}


ic::Vec2i ic::MouseController::get_cursor_position() {
    return this->cursorPosition;
}
ic::Vec2i ic::MouseController::get_relative_motion() {
    return this->relativeMotion;
}
float ic::MouseController::get_wheel_direction() {
    return this->scrollWheelDirection;
}

bool ic::MouseController::button_held_down() {
    return this->mouseHeldDown;
}