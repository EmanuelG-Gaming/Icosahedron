#ifndef IC_PHYSICS_OBJECT_3D_H
#define IC_PHYSICS_OBJECT_3D_H

#include <functional>

#include <Physics/3d/Transform3D.h>
#include <Physics/3d/Collider3D.h>

namespace ic { namespace Physics {
    struct Manifold3D;

    /** @brief An object in a 3D scene. */
    struct Object3D {
        Transform3D *transform;
        Collider3D *collider;
        
        std::function<void(ic::Physics::Manifold3D, float)> onCollision;

        bool dynamic;
        bool isTrigger;
        bool collides;
        
        Object3D();
        virtual ~Object3D() {}

        void collision(const std::function<void(ic::Physics::Manifold3D, float)> &to);

        void set_position(float x, float y, float z);
        void set_position(ic::Vec3f &to);
    };
}}

#endif