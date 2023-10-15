#ifndef IC_PHYSICS_RIGID_OBJECT_2D_H
#define IC_PHYSICS_RIGID_OBJECT_2D_H

#include <Icosahedron/math/geom/Vectors.h>

#include <Physics/2d/objects/Object2D.h>


namespace ic { namespace Physics {
    struct RigidObject2D : public Object2D {
        public:
            ic::Vec2f force, velocity;
            float torque, angularVelocity;

            float mass, inertia;
            float restitution;
            float staticFriction, dynamicFriction;
            
            ic::Vec2f gravity;
            
            RigidObject2D();

            
            void apply_velocity(float x, float y);
            void apply_velocity(ic::Vec2f &vel);

            void set_mass(float to);
    };
}}
#endif