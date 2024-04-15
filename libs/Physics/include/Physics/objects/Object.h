#ifndef IC_PHYSICS_OBJECT_H
#define IC_PHYSICS_OBJECT_H

#include <functional>

#include <Physics/Transform.h>
#include <Physics/Collider.h>


namespace ic { namespace Physics {
    struct Manifold;
    struct Collider;

    struct Object {
        Transform *transform;
        Collider *collider;
        
        std::function<void(ic::Physics::Manifold, float)> onCollision;

        bool dynamic;
        bool isTrigger;
        bool collides;
        
        Object();
        virtual ~Object() {}

        void collision(const std::function<void(ic::Physics::Manifold, float)> &to);

        void set_position(float x, float y, float z = 0.0f);
        void set_position(ic::Vec3 &to);
        void set_position(ic::Vec2 &to);
    };
}}

#endif