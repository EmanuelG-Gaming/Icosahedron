#ifndef IC_PHYSICS_WIND_FORCE_2D_H
#define IC_PHYSICS_WIND_FORCE_2D_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Physics/2d/forces/Force2D.h>


namespace ic { namespace Physics {
    class WindForce2D : public Force2D {
        public:
            ic::Vec2f force;
            float density;

            /** @brief Air resistance. */
            float coefficient;

            WindForce2D(float density, float coefficient);
            WindForce2D(ic::Vec2f force, float density = 1.0f, float coefficient = 1.0f);


            void apply(ic::Physics::Object2D *object) override;
    };
}}

#endif