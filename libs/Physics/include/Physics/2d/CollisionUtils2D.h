#ifndef IC_PHYSICS_COLLISION_UTILS_2D_H
#define IC_PHYSICS_COLLISION_UTILS_2D_H

#include <Physics/2d/ManifoldPoints2D.h>
#include <Physics/2d/Collider2D.h>
#include <Physics/2d/Transform2D.h>

namespace ic { namespace Physics {
    namespace CollisionUtils2D {
        ManifoldPoints2D circle_circle(CircleCollider *colliderA, Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB);
        ManifoldPoints2D rectangle_circle(RectangleCollider *colliderA, Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB);

        /** @brief AABB collisions. */
        ManifoldPoints2D rectangle_rectangle(RectangleCollider *colliderA, Transform2D *transformA, RectangleCollider *colliderB, Transform2D *transformB);
        ManifoldPoints2D polygon_circle(PolygonCollider *colliderA, Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB);
    };
}}
#endif