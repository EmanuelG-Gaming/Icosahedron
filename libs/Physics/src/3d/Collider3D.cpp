#include <Physics/3d/Collider3D.h>
#include <Physics/3d/CollisionUtils3D.h>


// Sphere collider
// ...............
ic::Physics::ManifoldPoints3D ic::Physics::SphereCollider::test(Transform3D *transformA, Collider3D *colliderB, Transform3D *transformB) {
    return colliderB->test(transformB, this, transformA);
}

ic::Physics::ManifoldPoints3D ic::Physics::SphereCollider::test(Transform3D *transformA, SphereCollider *colliderB, Transform3D *transformB) {
    return ic::Physics::CollisionUtils3D::sphere_sphere(this, transformA, colliderB, transformB);
}

ic::Physics::ManifoldPoints3D ic::Physics::SphereCollider::test(Transform3D *transformA, BoxCollider *colliderB, Transform3D *transformB) {
    // Reuse code
    ic::Physics::ManifoldPoints3D points = colliderB->test(transformB, this, transformA);
    
    // Swap the points, so that the collision might not break
    ic::Vec3f temporary = points.AtoB;
    points.AtoB = points.BtoA;
    points.BtoA = temporary;
    points.normal = points.normal * -1;
    
    return points;
}



// Box collider
// ............
ic::Physics::ManifoldPoints3D ic::Physics::BoxCollider::test(Transform3D *transformA, Collider3D *colliderB, Transform3D *transformB) {
    return colliderB->test(transformB, this, transformA);
}

ic::Physics::ManifoldPoints3D ic::Physics::BoxCollider::test(Transform3D *transformA, SphereCollider *colliderB, Transform3D *transformB) {
    return ic::Physics::CollisionUtils3D::box_sphere(this, transformA, colliderB, transformB);
}

ic::Physics::ManifoldPoints3D ic::Physics::BoxCollider::test(Transform3D *transformA, BoxCollider *colliderB, Transform3D *transformB) {
    return ic::Physics::CollisionUtils3D::box_box(this, transformA, colliderB, transformB);
}