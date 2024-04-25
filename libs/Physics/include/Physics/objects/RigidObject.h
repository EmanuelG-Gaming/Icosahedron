#ifndef IC_PHYSICS_RIGID_OBJECT_H
#define IC_PHYSICS_RIGID_OBJECT_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Physics/objects/Object.h>


namespace ic { namespace Physics {
    struct RigidObject : public Object {
        public:
            ic::Vec3 force, velocity;

            float mass;
            
            /** @brief Precomputed value for the quotient of the object's mass. */
            float inverseMass;
        
            float restitution;
            float staticFriction, dynamicFriction;
            
            ic::Vec3 gravity;
            
            RigidObject();

            
            ic::Physics::RigidObject *apply_velocity(float x, float y, float z = 0.0f);
            ic::Physics::RigidObject *apply_velocity(ic::Vec3 &vel);
            ic::Physics::RigidObject *apply_velocity(ic::Vec2 &vel);

            ic::Physics::RigidObject *set_mass(float to);
    };
}}
#endif