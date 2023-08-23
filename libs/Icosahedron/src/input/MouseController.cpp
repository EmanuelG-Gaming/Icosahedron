#include <Icosahedron/input/MouseController.h>

using namespace ic;

MouseController::MouseController() {

}

void ic::MouseController::update(float dt) {
    
}

void ic::MouseController::handle_event(ic::Event event, float dt) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        for (auto &action : inputActions) {
            if (action.type != MOUSE_INPUT_DOWN) continue;

            action.callback();
        }
    }

    if (event.type == SDL_MOUSEBUTTONUP) {
        for (auto &action : inputActions) {
            if (action.type != MOUSE_INPUT_UP) continue;

            action.callback();
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

            
ic::Vec2i ic::MouseController::get_cursor_position() {
    return this->cursorPosition;
}
ic::Vec2i ic::MouseController::get_relative_motion() {
    return this->relativeMotion;
}
