#ifndef IC_PHYSICS_COLLIDER_2D_H
#define IC_PHYSICS_COLLIDER_2D_H

#include <Physics/2d/ManifoldPoints2D.h>
#include <Physics/2d/Transform2D.h>

namespace ic { namespace Physics {
    struct Collider2D;
    struct CircleCollider;

    struct Collider2D {
        Collider2D() {}

        virtual ManifoldPoints2D test(Transform2D *TransformA, Collider2D *colliderB, Transform2D *transformB) = 0;
        virtual ManifoldPoints2D test(Transform2D *TransformA, CircleCollider *colliderB, Transform2D *TransformB) = 0;
    };

    struct CircleCollider : Collider2D {
        public:
            float radius = 1.0f;
            CircleCollider() {}
            CircleCollider(float radius) : radius(radius) {}

            ManifoldPoints2D test(Transform2D *transformA, Collider2D *colliderB, Transform2D *transformB) override;
            ManifoldPoints2D test(Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB) override;
    };
}}

#endif