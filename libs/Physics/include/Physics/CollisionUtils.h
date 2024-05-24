#ifndef IC_PHYSICS_COLLISION_UTILS_H
#define IC_PHYSICS_COLLISION_UTILS_H

#include <Physics/Manifold.h>
#include <Physics/Collider.h>
#include <Physics/Transform.h>


namespace ic { namespace Physics {
    enum class OverlapDirections {
        X, Y, Z
    };
    
    namespace CollisionUtils {
        ManifoldPoints sphere_sphere(SphereCollider *colliderA, Transform *transformA, SphereCollider *colliderB, Transform *transformB);
        ManifoldPoints box_sphere(BoxCollider *colliderA, Transform *transformA, SphereCollider *colliderB, Transform *transformB);
        ManifoldPoints box_box(BoxCollider *colliderA, Transform *transformA, BoxCollider *colliderB, Transform *transformB);
        ManifoldPoints polygon_sphere(PolygonCollider *colliderA, Transform *transformA, SphereCollider *colliderB, Transform *transformB);
        
        /** @brief Separating Axis Theorem. */
        //ManifoldPoints polygon_polygon_SAT(PolygonCollider *colliderA, Transform *transformA, PolygonCollider *colliderB, Transform *transformB);
    };
}}
#endif