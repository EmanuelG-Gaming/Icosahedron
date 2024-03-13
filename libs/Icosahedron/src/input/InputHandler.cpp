#include <Icosahedron/input/InputHandler.h>
#include <Icosahedron/util/Time.h>

void ic::InputHandler::handle(ic::Event event) {
    for (auto pair : inputs) {
        pair.second->handle_event(event, ic::Time::globalDelta);
    }
}
void ic::InputHandler::update() {
    for (auto pair : inputs) {
        pair.second->update(ic::Time::globalDelta);
    }
}
            
void ic::InputHandler::add_input(ic::Input* input, const INPUT_LOCATION &location) {
    inputs[location] = input;
}
ic::Input *ic::InputHandler::find_input(const INPUT_LOCATION &location) {
    return inputs[location];
}

ic::KeyboardController *ic::InputHandler::find_keyboard(const INPUT_LOCATION &location) {
    return static_cast<ic::KeyboardController*>(find_input(location));
}
ic::MouseController *ic::InputHandler::find_mouse(const INPUT_LOCATION &location) {
    return static_cast<ic::MouseController*>(find_input(location));
}