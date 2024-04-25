#ifndef IC_PHYSICS_COLLIDER_H
#define IC_PHYSICS_COLLIDER_H

#include <Physics/ManifoldPoints.h>
#include <Physics/Transform.h>
#include <vector>


namespace ic { namespace Physics {
    struct Collider;
    struct SphereCollider;
    struct BoxCollider;
    struct PolygonCollider;

    struct Collider {
        Collider() {}

        virtual ManifoldPoints test(Transform *TransformA, Collider *colliderB, Transform *transformB) = 0;
        virtual ManifoldPoints test(Transform *TransformA, SphereCollider *colliderB, Transform *TransformB) = 0;
        virtual ManifoldPoints test(Transform *TransformA, BoxCollider *colliderB, Transform *TransformB) = 0;
        virtual ManifoldPoints test(Transform *TransformA, PolygonCollider *colliderB, Transform *TransformB) = 0;
    };

    struct SphereCollider : Collider {
        public:
            float radius = 1.0f;

            SphereCollider() {}
            SphereCollider(float radius) : radius(radius) {}

            ManifoldPoints test(Transform *TransformA, Collider *colliderB, Transform *transformB) override;
            ManifoldPoints test(Transform *TransformA, SphereCollider *colliderB, Transform *TransformB) override;
            ManifoldPoints test(Transform *TransformA, BoxCollider *colliderB, Transform *TransformB) override;
            ManifoldPoints test(Transform *TransformA, PolygonCollider *colliderB, Transform *TransformB) override;
    };

    struct BoxCollider : Collider {
        public:
            float width = 1.0f, height = 1.0f, depth = 1.0f;
            
            BoxCollider() {}
            BoxCollider(float width, float height, float depth = 0.0f) : width(width), height(height), depth(depth) {}

            ManifoldPoints test(Transform *TransformA, Collider *colliderB, Transform *transformB) override;
            ManifoldPoints test(Transform *TransformA, SphereCollider *colliderB, Transform *TransformB) override;
            ManifoldPoints test(Transform *TransformA, BoxCollider *colliderB, Transform *TransformB) override;
            ManifoldPoints test(Transform *TransformA, PolygonCollider *colliderB, Transform *TransformB) override;
    };

    struct PolygonCollider : Collider {
        public:
            std::vector<ic::Vec2> points;
            
            PolygonCollider() {}
            PolygonCollider(const std::vector<ic::Vec2> &points) : points(points) {}
            PolygonCollider(const std::vector<float> &unpacked) {
                for (int i = 0; i < unpacked.size(); i += 2) {
                    ic::Vec2 p(unpacked[i], unpacked[i + 1]);
                    points.push_back(p);
                }
            }

            ManifoldPoints test(Transform *TransformA, Collider *colliderB, Transform *transformB) override;
            ManifoldPoints test(Transform *TransformA, SphereCollider *colliderB, Transform *TransformB) override;
            ManifoldPoints test(Transform *TransformA, BoxCollider *colliderB, Transform *TransformB) override;
            ManifoldPoints test(Transform *TransformA, PolygonCollider *colliderB, Transform *TransformB) override;
    };
}}

#endif