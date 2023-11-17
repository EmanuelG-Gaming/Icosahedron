#include <Physics/2d/CollisionUtils2D.h>
#include <Icosahedron/math/geom/Raycast.h>

ic::Physics::ManifoldPoints2D ic::Physics::CollisionUtils2D::circle_circle(CircleCollider *colliderA, Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB) {
    float distance2 = transformA->position.dst2(transformB->position);
    float radius = colliderA->radius;
    float radiusOther = colliderB->radius;

    bool intersecting = distance2 <= (radius + radiusOther) * (radius + radiusOther);
    if (!intersecting || distance2 < 0.001f) {
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


ic::Physics::ManifoldPoints2D ic::Physics::CollisionUtils2D::rectangle_circle(RectangleCollider *colliderA, Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB) {
    float width = colliderA->width;
    float height = colliderA->height;
 
    ic::Vec2f pointing = (transformA->position - transformB->position).rotate(-transformA->angle);
    if ((transformB->position.x() >= transformA->position.x() - width && transformB->position.x() <= transformA->position.x() + width) &&
        (transformB->position.y() >= transformA->position.y() - height && transformB->position.y() <= transformA->position.y() + height)) {
        // If the circle's center is inside the rectangle
        pointing = pointing * 100000.0f;
    }

    ic::Vec2f clamped = pointing.clamp(ic::Vec2f({ -width, -height }), ic::Vec2f({ width, height }));

    ic::Vec2f closest = clamped.rotate(transformA->angle) + transformB->position;

    float distance2 = closest.dst2(transformA->position);
    bool intersecting = distance2 <= (colliderB->radius * colliderB->radius);
    if (!intersecting || distance2 < 0.001f) {
        return ic::Physics::ManifoldPoints2D();
    }

    ic::Vec2f AtoB = (closest - transformA->position).nor() * colliderB->radius + transformA->position;
    ic::Vec2f BtoA = closest;
             
    ic::Vec2f dir = BtoA - AtoB;
    ic::Vec2f normal = dir.nor();

    return ic::Physics::ManifoldPoints2D(AtoB, BtoA, normal, dir.len());
}


ic::Physics::ManifoldPoints2D ic::Physics::CollisionUtils2D::polygon_circle(PolygonCollider *colliderA, Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB) {
    // Transforms global coordinates into model-space coordinates
    ic::Vec2f positionToModel = (transformB->position - transformA->position);//.rotate(-transformA->angle);

    // Broad-phase checking
    ic::Rectangle tmp = { positionToModel, { colliderB->radius, colliderB->radius } };
    if (!colliderA->bounding.overlaps(tmp)) {
        return ic::Physics::ManifoldPoints2D();
    }

    int rayIntersections = 0;
    float closestDistance2 = 10000.0f;
    ic::Vec2f closestPoint;

    for (int i = 0; i < colliderA->modelVertices.size(); i++) {
        int next = i + 1;
        if (next == colliderA->modelVertices.size()) next = 0;

        ic::Vec2f sidePointA = colliderA->modelVertices[i] + transformA->position;
        ic::Vec2f sidePointB = colliderA->modelVertices[next] + transformA->position;

        
        bool collides = ic::Raycast::get().line_segments_collide(100000.0f, transformB->position.y(), transformB->position.x(), transformB->position.y(),
                                                                sidePointA.x(), sidePointA.y(), sidePointB.x(), sidePointB.y());
        
        // Add 1 if the horizontal ray intersects the polygon side, 0 otherwise
        rayIntersections += (int) collides;
    }

    // Get closest point
    for (int i = 0; i < colliderA->modelVertices.size(); i++) {
        int next = i + 1;
        if (next == colliderA->modelVertices.size()) next = 0;

        ic::Vec2f sidePointA = colliderA->modelVertices[i] + transformA->position;
        ic::Vec2f sidePointB = colliderA->modelVertices[next] + transformA->position;

        ic::Vec2f closest = ic::Raycast::get().get_closest_point(transformB->position, sidePointA, sidePointB);
        float distanceCompare = closest.dst2(transformB->position);
    
        if (distanceCompare < closestDistance2) {
            closestPoint = closest;
            closestDistance2 = distanceCompare;
        }
    }

    if ((closestDistance2 > colliderB->radius * colliderB->radius)) return ic::Physics::ManifoldPoints2D();

    ic::Vec2f normal = (closestPoint - transformB->position) * colliderB->radius;
    float length = normal.len();
    normal = normal.nor();

    //ic::Vec2f AtoB = transformB->position + normal * colliderB->radius;
    //ic::Vec2f BtoA = closestPoint;
    ic::Vec2f AtoB = { 0.0f, 0.0f };
    ic::Vec2f BtoA = { 0.0f, 0.0f };
    
    return ic::Physics::ManifoldPoints2D(AtoB, BtoA, normal, length);
}

//ic::Physics::ManifoldPoints2D ic::Physics::CollisionUtils2D::spMass2D_circle(SpringMassCollider2D *colliderA, Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB) {
//
//}
//
//
//ic::Physics::ManifoldPoints2D ic::Physics::CollisionUtils2D::spMass2D_rectangle(SpringMassCollider2D *colliderA, Transform2D *transformA, RectangleCollider *colliderB, Transform2D *transformB) {
//
//}
            