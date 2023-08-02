#include <Icosahedron/input/InputHandler.h>

void ic::InputHandler::handle(ic::Event event, float dt) {
    for (auto pair : inputs) {
         pair.second->handle_event(event, dt);
    }
}
void ic::InputHandler::update(float dt) {
    for (auto pair : inputs) {
         pair.second->update(dt);
    }
}
            
void ic::InputHandler::add_input(ic::Input* input, const INPUT_LOCATION &location) {
    inputs[location] = input;
}
ic::Input *ic::InputHandler::find_input(const INPUT_LOCATION &location) {
    return inputs[location];
}