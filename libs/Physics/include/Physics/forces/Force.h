#ifndef IC_PHYSICS_FORCE_H
#define IC_PHYSICS_FORCE_H

#include <Physics/objects/Object.h>


namespace ic { namespace Physics {
    class Force {
        public:
            virtual void apply(ic::Physics::Object *object) {};
    };
}}

#endif