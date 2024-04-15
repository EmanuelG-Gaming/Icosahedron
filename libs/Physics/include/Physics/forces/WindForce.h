#ifndef IC_PHYSICS_WIND_FORCE_H
#define IC_PHYSICS_WIND_FORCE_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Physics/forces/Force.h>


namespace ic { namespace Physics {
    class WindForce : public Force {
        public:
            ic::Vec3 force;
            float density;

            /** @brief Air resistance. */
            float coefficient;
            
            WindForce();
            WindForce(float density, float coefficient);
            WindForce(ic::Vec3 force, float density = 1.0f, float coefficient = 1.0f);


            void apply(ic::Physics::Object *object) override;
    };
}}

#endif