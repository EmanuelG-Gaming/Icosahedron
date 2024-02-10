#ifndef IC_PHYSICS_COLLIDER_3D_H
#define IC_PHYSICS_COLLIDER_3D_H

#include <string>

#include <Icosahedron/math/geom/Polygon.h>
#include <Icosahedron/math/geom/Rectangle.h>

#include <Physics/3d/ManifoldPoints3D.h>
#include <Physics/3d/Transform3D.h>


namespace ic { namespace Physics {
    struct Collider3D;
    struct SphereCollider;
    struct BoxCollider;

    struct Collider3D {
        Collider3D() {}

        virtual ManifoldPoints3D test(Transform3D *TransformA, Collider3D *colliderB, Transform3D *transformB)           = 0;
        virtual ManifoldPoints3D test(Transform3D *TransformA, SphereCollider *colliderB, Transform3D *TransformB)       = 0;
        virtual ManifoldPoints3D test(Transform3D *TransformA, BoxCollider *colliderB, Transform3D *TransformB)    = 0;
    };

    struct SphereCollider : Collider3D {
        public:
            float radius = 1.0f;

            SphereCollider() {}
            SphereCollider(float radius) : radius(radius) {}

            ManifoldPoints3D test(Transform3D *TransformA, Collider3D *colliderB, Transform3D *transformB)           override;
            ManifoldPoints3D test(Transform3D *TransformA, SphereCollider *colliderB, Transform3D *TransformB)       override;
            ManifoldPoints3D test(Transform3D *TransformA, BoxCollider *colliderB, Transform3D *TransformB)    override;
    };

    struct BoxCollider : Collider3D {
        public:
            float width = 1.0f, height = 1.0f, depth = 1.0f;
            
            BoxCollider() {}
            BoxCollider(float width, float height, float depth) : width(width), height(height), depth(depth) {}

            ManifoldPoints3D test(Transform3D *TransformA, Collider3D *colliderB, Transform3D *transformB)           override;
            ManifoldPoints3D test(Transform3D *TransformA, SphereCollider *colliderB, Transform3D *TransformB)       override;
            ManifoldPoints3D test(Transform3D *TransformA, BoxCollider *colliderB, Transform3D *TransformB)    override;
    };
}}

#endif