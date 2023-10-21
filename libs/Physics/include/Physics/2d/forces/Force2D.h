#ifndef IC_PHYSICS_FORCE_2D_H
#define IC_PHYSICS_FORCE_2D_H

#include <Physics/2d/objects/Object2D.h>


namespace ic { namespace Physics {
    class Force2D {
        public:
            virtual void apply(ic::Physics::Object2D *object) {};
    };
}}

#endif