#ifndef IC_PHYSICS_GRAVITY_H
#define IC_PHYSICS_GRAVITY_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Physics/forces/Force.h>


namespace ic { namespace Physics {
    class Gravity : public Force {
        public:
            ic::Vec3 force;

            Gravity();
            Gravity(float x, float y, float z = 0.0f);
            Gravity(ic::Vec3 force);
            Gravity(ic::Vec2 force);

            void apply(ic::Physics::Object *object) override;
    };
}}

#endif