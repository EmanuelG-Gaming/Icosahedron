#include <Physics/2d/CollisionUtils2D.h>

ic::Physics::ManifoldPoints2D ic::Physics::CollisionUtils2D::circle_circle(CircleCollider *colliderA, Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB) {
    float distance2 = transformA->position.dst2(transformB->position);
    float radius = colliderA->radius;
    float radiusOther = colliderB->radius;

    bool intersecting = distance2 <= (radius + radiusOther) * (radius + radiusOther);
    if (!intersecting || distance2 < 0.00001f) {
        return ic::Physics::ManifoldPoints2D();
    }
    
    // Points on the circles
    Vec2f AtoB = (transformB->position - transformA->position).nor();
    Vec2f BtoA = AtoB * -1;
    AtoB = AtoB * radiusOther + transformA->position;
    BtoA = BtoA * radius + transformB->position;
    
    Vec2f direction = BtoA - AtoB;
    Vec2f normal = direction.nor();
    return ic::Physics::ManifoldPoints2D(AtoB, BtoA, normal, direction.len());
}