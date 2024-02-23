#ifndef IC_PHYSICS_COLLISION_UTILS_3D_H
#define IC_PHYSICS_COLLISION_UTILS_3D_H

#include <Physics/3d/ManifoldPoints3D.h>
#include <Physics/3d/Collider3D.h>
#include <Physics/3d/Transform3D.h>

namespace ic { namespace Physics {
    enum class OverlapDirections {
        X, Y, Z
    };
    
    namespace CollisionUtils3D {
        ManifoldPoints3D sphere_sphere(SphereCollider *colliderA, Transform3D *transformA, SphereCollider *colliderB, Transform3D *transformB);
        ManifoldPoints3D box_sphere(BoxCollider *colliderA, Transform3D *transformA, SphereCollider *colliderB, Transform3D *transformB);

        /** @brief AABB collisions. */
        ManifoldPoints3D box_box(BoxCollider *colliderA, Transform3D *transformA, BoxCollider *colliderB, Transform3D *transformB);
    };
}}
#endif