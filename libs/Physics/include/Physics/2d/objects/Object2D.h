#ifndef IC_PHYSICS_OBJECT_2D_H
#define IC_PHYSICS_OBJECT_2D_H

#include <functional>

#include <Physics/2d/Transform2D.h>
#include <Physics/2d/Collider2D.h>

namespace ic { namespace Physics {
    /** @brief An object in a 2D scene. */
    struct Object2D {
        Transform2D *transform;
        Collider2D *collider;
        
        int index;

        std::function<void()> onCollision;

        bool dynamic;
        bool isTrigger;

        Object2D();
        virtual ~Object2D() {}

        void collision(const std::function<void()> &to);

        void set_position(float x, float y);
        void set_position(ic::Vec2f &to);
    };
}}

#endif