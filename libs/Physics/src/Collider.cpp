#include <Physics/Collider.h>
#include <Physics/CollisionUtils.h>


// Sphere collider
// ...............
ic::Physics::ManifoldPoints ic::Physics::SphereCollider::test(Transform *transformA, Collider *colliderB, Transform *transformB) {
    return colliderB->test(transformB, this, transformA);
}

ic::Physics::ManifoldPoints ic::Physics::SphereCollider::test(Transform *transformA, SphereCollider *colliderB, Transform *transformB) {
    return ic::Physics::CollisionUtils::sphere_sphere(this, transformA, colliderB, transformB);
}

ic::Physics::ManifoldPoints ic::Physics::SphereCollider::test(Transform *transformA, BoxCollider *colliderB, Transform *transformB) {
    // Reuse code
    ic::Physics::ManifoldPoints points = colliderB->test(transformB, this, transformA);
    
    // Swap the points, so that the collision might not break
    ic::Vec3f temporary = points.AtoB;
    points.AtoB = points.BtoA;
    points.BtoA = temporary;
    points.normal = points.normal * -1;
    
    return points;
}

ic::Physics::ManifoldPoints ic::Physics::SphereCollider::test(Transform *transformA, PolygonCollider *colliderB, Transform *transformB) {
    // Reuse code
    ic::Physics::ManifoldPoints points = colliderB->test(transformB, this, transformA);
    
    // Swap the points, so that the collision might not break
    ic::Vec3f temporary = points.AtoB;
    points.AtoB = points.BtoA;
    points.BtoA = temporary;
    points.normal = points.normal * -1;
    
    return points;
}





// Box collider
// ............
ic::Physics::ManifoldPoints ic::Physics::BoxCollider::test(Transform *transformA, Collider *colliderB, Transform *transformB) {
    return colliderB->test(transformB, this, transformA);
}

ic::Physics::ManifoldPoints ic::Physics::BoxCollider::test(Transform *transformA, SphereCollider *colliderB, Transform *transformB) {
    return ic::Physics::CollisionUtils::box_sphere(this, transformA, colliderB, transformB);
}

ic::Physics::ManifoldPoints ic::Physics::BoxCollider::test(Transform *transformA, BoxCollider *colliderB, Transform *transformB) {
    return ic::Physics::CollisionUtils::box_box(this, transformA, colliderB, transformB);
}
ic::Physics::ManifoldPoints ic::Physics::BoxCollider::test(Transform *transformA, PolygonCollider *colliderB, Transform *transformB) {
    return ic::Physics::ManifoldPoints();
}


// Polygon collider
// ................
ic::Physics::ManifoldPoints ic::Physics::PolygonCollider::test(Transform *transformA, Collider *colliderB, Transform *transformB) {
    return colliderB->test(transformB, this, transformA);
}

ic::Physics::ManifoldPoints ic::Physics::PolygonCollider::test(Transform *transformA, SphereCollider *colliderB, Transform *transformB) {
    return ic::Physics::CollisionUtils::polygon_sphere(this, transformA, colliderB, transformB);
}

ic::Physics::ManifoldPoints ic::Physics::PolygonCollider::test(Transform *transformA, BoxCollider *colliderB, Transform *transformB) {
    return ic::Physics::ManifoldPoints();
}

ic::Physics::ManifoldPoints ic::Physics::PolygonCollider::test(Transform *transformA, PolygonCollider *colliderB, Transform *transformB) {
    return ic::Physics::ManifoldPoints();
}