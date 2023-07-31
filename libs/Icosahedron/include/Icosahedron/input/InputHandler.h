#ifndef IC_INPUT_HANDLER_H
#define IC_INPUT_HANDLER_H

#include <map>

#include <Icosahedron/input/Input.h>

namespace ic {
    class InputHandler {
        public:
            InputHandler() {}
            
            void handle(ic::Event event, float dt);
            void add_input(ic::Input* input, const INPUT_LOCATION &location);
            ic::Input &find_input(const INPUT_LOCATION &location);
            
        private:
            std::map<INPUT_LOCATION, ic::Input*> inputs;
    };
}
#endif