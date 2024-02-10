#ifndef IC_PHYSICS_RIGID_OBJECT_3D_H
#define IC_PHYSICS_RIGID_OBJECT_3D_H

#include <Icosahedron/math/geom/Vectors.h>

#include <Physics/3d/objects/Object3D.h>


namespace ic { namespace Physics {
    struct RigidObject3D : public Object3D {
        public:
            ic::Vec3f force, velocity;

            float mass, inertia;
            float restitution;
            float staticFriction, dynamicFriction;
            
            ic::Vec3f gravity;
            
            RigidObject3D();

            
            ic::Physics::RigidObject3D *apply_velocity(float x, float y, float z);
            ic::Physics::RigidObject3D *apply_velocity(ic::Vec3f &vel);

            ic::Physics::RigidObject3D *set_mass(float to);
    };
}}
#endif