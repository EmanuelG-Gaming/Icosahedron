#ifndef IC_PHYSICS_WIND_FORCE_3D_H
#define IC_PHYSICS_WIND_FORCE_3D_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Physics/3d/forces/Force3D.h>


namespace ic { namespace Physics {
    class WindForce3D : public Force3D {
        public:
            ic::Vec3f force;
            float density;

            /** @brief Air resistance. */
            float coefficient;
            
            WindForce3D();
            WindForce3D(float density, float coefficient);
            WindForce3D(ic::Vec3f force, float density = 1.0f, float coefficient = 1.0f);


            void apply(ic::Physics::Object3D *object) override;
    };
}}

#endif