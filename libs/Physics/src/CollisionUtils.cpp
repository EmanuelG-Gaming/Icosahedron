#include <Physics/CollisionUtils.h>
#include <Icosahedron/math/geom/Raycast.h>

ic::Physics::ManifoldPoints ic::Physics::CollisionUtils::sphere_sphere(SphereCollider *colliderA, Transform *transformA, SphereCollider *colliderB, Transform *transformB) {
    float distance2 = transformA->position.dst2(transformB->position);
    float radius = colliderA->radius;
    float radiusOther = colliderB->radius;

    bool intersecting = distance2 <= (radius + radiusOther) * (radius + radiusOther);
    if (!intersecting || distance2 < 0.001f) {
        return ic::Physics::ManifoldPoints();
    }

    // Points on the circles
    Vec3f AtoB = (transformB->position - transformA->position).nor();
    Vec3f BtoA = AtoB * -1;
    AtoB = AtoB * radiusOther + transformA->position;
    BtoA = BtoA * radius + transformB->position;
    
    Vec3f direction = BtoA - AtoB;
    Vec3f normal = direction.nor();

    return ic::Physics::ManifoldPoints(AtoB, BtoA, normal, direction.len());
}


ic::Physics::ManifoldPoints ic::Physics::CollisionUtils::box_sphere(BoxCollider *colliderA, Transform *transformA, SphereCollider *colliderB, Transform *transformB) {
    float width = colliderA->width;
    float height = colliderA->height;
    float depth = colliderA->depth;

    ic::Vec3f pointing = (transformA->position - transformB->position);
    pointing = transformA->rotation.transform(pointing);

    if ((pointing.x() >= -width && pointing.x() <= width) &&
        (pointing.y() >= -height && pointing.y() <= height) &&
        (pointing.z() >= -depth && pointing.z() <= depth)) {
        // If the circle's center is inside the rectangle
        pointing = pointing * 1000.0f;
    }

    ic::Vec3f clamped = pointing.clamp(ic::Vec3f({ -width, -height, -depth }), ic::Vec3f({ width, height, depth }));

    // The rotation quaternion's conjugate is the same as inverse, because it has the length of 1
    ic::Vec3f closest = ic::Quaternion(transformA->rotation).conjugate().transform(clamped) + transformB->position;

    float distance2 = closest.dst2(transformA->position);
    bool intersecting = distance2 <= (colliderB->radius * colliderB->radius);
    if (!intersecting || distance2 < 0.001f) {
        return ic::Physics::ManifoldPoints();
    }

    ic::Vec3f AtoB = (closest - transformA->position).nor() * colliderB->radius + transformA->position;
    ic::Vec3f BtoA = closest;
             
    ic::Vec3f dir = BtoA - AtoB;
    ic::Vec3f normal = dir.nor();

    return ic::Physics::ManifoldPoints(AtoB, BtoA, normal, dir.len());
}

ic::Physics::ManifoldPoints ic::Physics::CollisionUtils::box_box(BoxCollider *colliderA, Transform *transformA, BoxCollider *colliderB, Transform *transformB) {
    float x1 = transformA->position.x() - colliderA->width, y1 = transformA->position.y() - colliderA->height, z1 = transformA->position.z() - colliderA->depth;
    float x2 = transformA->position.x() + colliderA->width, y2 = transformA->position.y() + colliderA->height, z2 = transformA->position.z() + colliderA->depth;
    
    float x3 = transformB->position.x() - colliderB->width, y3 = transformB->position.y() - colliderB->height, z3 = transformB->position.z() - colliderB->depth;
    float x4 = transformB->position.x() + colliderB->width, y4 = transformB->position.y() + colliderB->height, z4 = transformB->position.z() + colliderB->depth;

    bool overlaps = (x2 >= x3 && x1 <= x4) &&
                    (y2 >= y3 && y1 <= y4) &&
                    (z2 >= z3 && z1 <= z4);

    if (!overlaps) return ic::Physics::ManifoldPoints();

    ic::Vec3f overlap;
    overlap.x() = x2 > x4 ? x4 - x1 : x2 - x3;
    overlap.y() = y2 > y4 ? y4 - y1 : y2 - y3;
    overlap.z() = z2 > z4 ? z4 - z1 : z2 - z3;

    ic::Vec3f normal;
    
    float leastOverlap = 0.0f;
    OverlapDirections dir;

    // Find the least amount of overlap
    if (overlap.x() < overlap.y()) {
        leastOverlap = overlap.x();
        dir = OverlapDirections::X;
    } else {
        leastOverlap = overlap.y();
        dir = OverlapDirections::Y;
    }

    if (overlap.z() < leastOverlap) {
        leastOverlap = overlap.z();
        dir = OverlapDirections::Z;
    }


    {
        float difference = 0.0f;
        if (dir == OverlapDirections::X) difference = transformA->position.x() - transformB->position.x();
        else if (dir == OverlapDirections::Y) difference = transformA->position.y() - transformB->position.y();
        else if (dir == OverlapDirections::Z) difference = transformA->position.z() - transformB->position.z();

        int sign = (difference >= 0.0f) ? 1 : -1;
        if (dir == OverlapDirections::X) normal.x() = sign * overlap.x();
        else if (dir == OverlapDirections::Y) normal.y() = sign * overlap.y();
        else if (dir == OverlapDirections::Z) normal.z() = sign * overlap.z();
    }

    float length = normal.len();
    normal = normal.nor();

    return ic::Physics::ManifoldPoints(normal, length);
}

ic::Physics::ManifoldPoints ic::Physics::CollisionUtils::polygon_sphere(PolygonCollider *colliderA, Transform *transformA, SphereCollider *colliderB, Transform *transformB) {
    ic::Vec3 ballPosTmp(transformB->position - transformA->position);
    ic::Vec3 ballPos = transformA->rotation.transform(ballPosTmp);


    int intersectionCount = 0;
    int maximumIndex = colliderA->points.size() - 1;

    for (int i = 1; i < colliderA->points.size(); i++) {
        ic::Vec2 prev = colliderA->points[i - 1], next = colliderA->points[i];
        if (ic::Raycast::line_segments_collide(prev.x(), prev.y(), next.x(), next.y(), -10000.0f, ballPos.y(), ballPos.x(), ballPos.y())) {
            intersectionCount++;
        }
    }

    // Now do the same for the first and last points
    {
        ic::Vec2 prev = colliderA->points[maximumIndex], next = colliderA->points[0];
        if (ic::Raycast::line_segments_collide(prev.x(), prev.y(), next.x(), next.y(), -10000.0f, ballPos.y(), ballPos.x(), ballPos.y())) {
            intersectionCount++;
        }
    }


    Vec2f closest;
    float closestDistance = 0.0f;
    ic::Vec2 bpos2D = ic::Vec2(ballPos.x(), ballPos.y());
    for (int i = 1; i < colliderA->points.size(); i++) {
        Vec2f p1 = colliderA->points[i - 1], p2 = colliderA->points[i];
        
        Vec2f compare = ic::Raycast::get_closest_point_line(bpos2D, p1, p2);
        float distance2 = compare.dst2(bpos2D);
        if (closestDistance == 0.0f || distance2 < closestDistance) {
            closest = compare;
            closestDistance = distance2;
        }
    }

    {
        Vec2f p1 = colliderA->points[maximumIndex], p2 = colliderA->points[0];
        
        Vec2f compare = ic::Raycast::get_closest_point_line(bpos2D, p1, p2);
        float distance2 = compare.dst2(bpos2D);
        if (closestDistance == 0.0f || distance2 < closestDistance) {
            closest = compare;
            closestDistance = distance2;
        }
    }

    closestDistance = sqrt(closestDistance);

    bool odd = intersectionCount % 2 == 1;
    if (odd || closestDistance > colliderB->radius) {
        return ic::Physics::ManifoldPoints();
    }



    ballPos = ballPos + transformA->position;
    Vec2f to = (closest - bpos2D);
    to = ic::Quaternion(transformA->rotation).conjugate().transform(to);


    ic::Vec2 normal = to.nor();
    if (odd) normal = -normal;

    float normalLength = std::max(closestDistance - colliderB->radius, 0.001f);

    return ic::Physics::ManifoldPoints(ic::Vec3(normal.x(), normal.y(), 0.0f), normalLength);
}

/*
ic::Physics::ManifoldPoints ic::Physics::CollisionUtils::polygon_polygon_SAT(PolygonCollider *colliderA, Transform *transformA, PolygonCollider *colliderB, Transform *transformB) {
    
}
*/