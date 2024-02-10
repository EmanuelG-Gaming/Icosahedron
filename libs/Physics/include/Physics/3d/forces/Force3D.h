#ifndef IC_PHYSICS_FORCE_3D_H
#define IC_PHYSICS_FORCE_3D_H

#include <Physics/3d/objects/Object3D.h>


namespace ic { namespace Physics {
    class Force3D {
        public:
            virtual void apply(ic::Physics::Object3D *object) {};
    };
}}

#endif