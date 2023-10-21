#ifndef IC_PHYSICS_GRAVITY_2D_H
#define IC_PHYSICS_GRAVITY_2D_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Physics/2d/forces/Force2D.h>


namespace ic { namespace Physics {
    class Gravity2D : public Force2D {
        public:
            ic::Vec2f force;

            Gravity2D();
            Gravity2D(float x, float y);
            Gravity2D(ic::Vec2f force);

            void apply(ic::Physics::Object2D *object) override;
    };
}}

#endif