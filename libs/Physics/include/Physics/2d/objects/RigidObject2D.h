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
            
            /** @returns The area of a cross-section with a plane perpendicular to a direction. */
            std::function<float(ic::Vec2f)> crossSectionalArea;

            RigidObject2D();

            
            ic::Physics::RigidObject2D *apply_velocity(float x, float y);
            ic::Physics::RigidObject2D *apply_velocity(ic::Vec2f &vel);

            ic::Physics::RigidObject2D *set_mass(float to);
            
            ic::Physics::RigidObject2D *set_section_area(const std::function<float(ic::Vec2f)> &crossSectionalArea);
    };
}}
#endif