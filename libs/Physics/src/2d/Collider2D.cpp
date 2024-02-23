#include <Physics/2d/Collider2D.h>
#include <Physics/2d/CollisionUtils2D.h>


// Circle collider

ic::Physics::ManifoldPoints2D ic::Physics::CircleCollider::test(Transform2D *transformA, Collider2D *colliderB, Transform2D *transformB) {
    return colliderB->test(transformB, this, transformA);
}

ic::Physics::ManifoldPoints2D ic::Physics::CircleCollider::test(Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB) {
    return ic::Physics::CollisionUtils2D::circle_circle(this, transformA, colliderB, transformB);
}

ic::Physics::ManifoldPoints2D ic::Physics::CircleCollider::test(Transform2D *transformA, RectangleCollider *colliderB, Transform2D *transformB) {
    // Reuse code
    ic::Physics::ManifoldPoints2D points = colliderB->test(transformB, this, transformA);
    
    // Swap the points, so that the collision might not break
    ic::Vec2f temporary = points.AtoB;
    points.AtoB = points.BtoA;
    points.BtoA = temporary;
    points.normal = points.normal * -1;
    
    return points;
}

ic::Physics::ManifoldPoints2D ic::Physics::CircleCollider::test(Transform2D *transformA, PolygonCollider *colliderB, Transform2D *transformB) {
    // Reuse code
    ic::Physics::ManifoldPoints2D points = colliderB->test(transformB, this, transformA);
    
    // Swap the points, so that the collision might not break
    ic::Vec2f temporary = points.AtoB;
    points.AtoB = points.BtoA;
    points.BtoA = temporary;
    points.normal = points.normal * -1;
    
    return points;
}

// Rectangle collider

ic::Physics::ManifoldPoints2D ic::Physics::RectangleCollider::test(Transform2D *transformA, Collider2D *colliderB, Transform2D *transformB) {
    return colliderB->test(transformB, this, transformA);
}

ic::Physics::ManifoldPoints2D ic::Physics::RectangleCollider::test(Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB) {
    return ic::Physics::CollisionUtils2D::rectangle_circle(this, transformA, colliderB, transformB);
}

ic::Physics::ManifoldPoints2D ic::Physics::RectangleCollider::test(Transform2D *transformA, RectangleCollider *colliderB, Transform2D *transformB) {
    return ic::Physics::CollisionUtils2D::rectangle_rectangle(this, transformA, colliderB, transformB);
}

ic::Physics::ManifoldPoints2D ic::Physics::RectangleCollider::test(Transform2D *transformA, PolygonCollider *colliderB, Transform2D *transformB) {
    return ic::Physics::ManifoldPoints2D();
}


// Polygon collider

ic::Physics::ManifoldPoints2D ic::Physics::PolygonCollider::test(Transform2D *transformA, Collider2D *colliderB, Transform2D *transformB) {
    return colliderB->test(transformB, this, transformA);
}

ic::Physics::ManifoldPoints2D ic::Physics::PolygonCollider::test(Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB) {
    return ic::Physics::CollisionUtils2D::polygon_circle(this, transformA, colliderB, transformB);
}

ic::Physics::ManifoldPoints2D ic::Physics::PolygonCollider::test(Transform2D *transformA, RectangleCollider *colliderB, Transform2D *transformB) {
    return ic::Physics::ManifoldPoints2D();
}

ic::Physics::ManifoldPoints2D ic::Physics::PolygonCollider::test(Transform2D *transformA, PolygonCollider *colliderB, Transform2D *transformB) {
    return ic::Physics::ManifoldPoints2D();
}