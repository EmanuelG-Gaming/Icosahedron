#ifndef IC_WASD_CONTROLLER_H
#define IC_WASD_CONTROLLER_H

#include <functional>
#include <map>

#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/input/Input.h>

namespace ic {
    /* An input that uses the WASD keys to get a direction vector.
     * Also allows for more keyboard actions to be added. */
    struct WASDController : public Input {
        public:
            ic::Vec2i direction;
            WASDController();

            void update(float dt) override;

            void add_action(const std::function<void(ic::WASDController*)> &callback, Uint8 location);

            Uint8* get_keyboard_state();
            ic::Vec2i get_direction();

        protected:
            Uint8* keyboardState;
            std::map<Uint8, std::function<void(ic::WASDController*)>> inputActions;
    };
}
#endif