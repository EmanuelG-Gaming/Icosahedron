#include <Physics/2d/Collider2D.h>
#include <Physics/2d/CollisionUtils2D.h>


// Circles

ic::Physics::ManifoldPoints2D ic::Physics::CircleCollider::test(Transform2D *transformA, Collider2D *colliderB, Transform2D *transformB) {
    return colliderB->test(transformB, this, transformA);
}

ic::Physics::ManifoldPoints2D ic::Physics::CircleCollider::test(Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB) {
    return ic::Physics::CollisionUtils2D::get().circle_circle(this, transformA, colliderB, transformB);
}

ic::Physics::ManifoldPoints2D ic::Physics::CircleCollider::test(Transform2D *transformA, PolygonCollider *colliderB, Transform2D *transformB) {
    return ic::Physics::ManifoldPoints2D();
}


// Polygons

ic::Physics::ManifoldPoints2D ic::Physics::PolygonCollider::test(Transform2D *transformA, Collider2D *colliderB, Transform2D *transformB) {
    return colliderB->test(transformB, this, transformA);
}

ic::Physics::ManifoldPoints2D ic::Physics::PolygonCollider::test(Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB) {
    return ic::Physics::ManifoldPoints2D();
}

ic::Physics::ManifoldPoints2D ic::Physics::PolygonCollider::test(Transform2D *transformA, PolygonCollider *colliderB, Transform2D *transformB) {
    return ic::Physics::CollisionUtils2D::get().polygon_polygon_separating_axis(this, transformA, colliderB, transformB);
}