#ifndef IC_PHYSICS_GRAVITY_3D_H
#define IC_PHYSICS_GRAVITY_3D_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Physics/3d/forces/Force3D.h>


namespace ic { namespace Physics {
    class Gravity3D : public Force3D {
        public:
            ic::Vec3f force;

            Gravity3D();
            Gravity3D(float x, float y, float z);
            Gravity3D(ic::Vec3f force);

            void apply(ic::Physics::Object3D *object) override;
    };
}}

#endif