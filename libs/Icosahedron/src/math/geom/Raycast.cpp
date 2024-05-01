#include <Icosahedron/math/geom/Raycast.h>
#include <algorithm>

bool ic::Raycast::line_segments_collide(float x1, float y1, float x2, float y2,
                            float x3, float y3, float x4, float y4) {
    float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    bool linesParallelOrCoincident = denominator == 0.0f;
    if (linesParallelOrCoincident) {
        return false;
    }
    float t = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
    float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3));
    
    t /= denominator;
    u /= denominator;

    bool intersects = t > 0 && t < 1 && u > 0 && u < 1;
    return intersects; 
}

template <std::size_t dims>
ic::RaycastHit<dims> ic::Raycast::ray_nsphere(ic::Vector<float, dims> sphereCenter, float radius, const ic::Vector<float, dims> &rayOrigin, const ic::Vector<float, dims> &rayDir) {
    using VecN = ic::Vector<float, dims>;

    VecN offset = rayOrigin - sphereCenter;

    // Form a 2nd order polynominal ax^2 + bx + c
    float a = rayDir.dot(rayDir);
    float b = 2.0f * rayDir.dot(offset);
    float c = offset.dot(offset) - radius * radius;

    float discriminant = b*b - 4.0f*a*c;

    ic::RaycastHit<dims> result;
    if (discriminant >= 0.0f) {
        float alpha1 = (-b + sqrt(discriminant)) / (2.0f * a);
        float alpha2 = -b / a - alpha2;

        // Sort hit points based on depth
        if (alpha2 < alpha1) std::swap(alpha1, alpha2);

        // Precomputes ray intersections
        result.hitAlphas[0] = alpha1;
        result.hitAlphas[1] = alpha2;
        result.hitPos[0] = (rayOrigin + rayDir * alpha1);
        result.hitPos[1] = (rayOrigin + rayDir * alpha2);
        result.hitNormals[0] = (sphereCenter - result.hitPos[0]).nor();
        result.hitNormals[1] = (sphereCenter - result.hitPos[1]).nor();

        result.collided = true;

        return result;
    }

    result.collided = false;
    return result;
}

ic::RaycastHit<3> ic::Raycast::ray_box(ic::Vec3 boxLower, ic::Vec3f boxUpper, const ic::Vec3 &rayOrigin, const ic::Vec3 &rayDir) {
    ic::RaycastHit<3> result;
    
    ic::Vec3f alphaNear = (boxLower - rayOrigin) / rayDir, alphaFar = (boxUpper - rayOrigin) / rayDir;

    if (alphaNear.x() > alphaFar.x()) std::swap(alphaNear.x(), alphaFar.x());
    if (alphaNear.y() > alphaFar.y()) std::swap(alphaNear.y(), alphaFar.y());
    if (alphaNear.z() > alphaFar.z()) std::swap(alphaNear.z(), alphaFar.z());

    if (alphaNear.x() > alphaFar.y() || alphaNear.y() > alphaFar.x()) {
        result.collided = false;
        return result;
    }

    float hitNear = std::max({ alphaNear.x(), alphaNear.y(), alphaNear.z() });
    float hitFar = std::min({ alphaFar.x(), alphaFar.y(), alphaFar.z() });

    if (hitFar < 0.0f) {
        result.collided = false;
        return result;
    }

    result.hitAlphas[0] = hitNear;
    result.hitAlphas[1] = hitFar;
    result.hitPos[0] = (rayOrigin + rayDir * hitNear);
    result.hitPos[1] = (rayOrigin + rayDir * hitFar);
    result.hitNormals[0] = ic::Vectors::up;
    result.hitNormals[1] = ic::Vectors::up;

    result.collided = true;
    return result;
}

ic::RaycastPlaneHit ic::Raycast::ray_plane(ic::Vec3 planeOrigin, ic::Vec3 planeNormal, const ic::Vec3 &rayOrigin, const ic::Vec3 &rayDir) {
    float denominator = planeNormal.dot(rayDir);

    ic::RaycastPlaneHit result;
    
    // Check if the ray's direction is facing the plane
    if (denominator <= -0.001f) {
        ic::Vec3 difference = planeOrigin - rayOrigin;
        float alpha = difference.dot(planeNormal) / denominator;

        bool collided = (alpha >= 0.0f);
        if (collided) {
            result.rayIntersect.hitAlphas[0] = alpha;
            result.rayIntersect.hitPos[0] = (rayOrigin + rayDir * alpha);
            result.rayIntersect.hitNormals[0] = planeNormal;

            // Get the plane's bitangents
            ic::Vec3f bt1 = planeNormal.crs(ic::Vectors::right);
            ic::Vec3f bt2 = bt1.crs(planeNormal);
            ic::Vec3f pointToOrigin = result.rayIntersect.hitPos[0] - planeOrigin;

            result.u = pointToOrigin.dot(bt1);
            result.v = pointToOrigin.dot(bt2);
        }

        result.rayIntersect.collided = collided;
        return result;
    }

    result.rayIntersect.collided = false;
    return result;
}

        
ic::RaycastHit<3> ic::Raycast::ray_sphere(ic::Vec3 sphereCenter, float radius, const ic::Vec3 &rayOrigin, const ic::Vec3 &rayDir) {
    return ic::Raycast::ray_nsphere<3>(sphereCenter, radius, rayOrigin, rayDir);
}

ic::Vec2f ic::Raycast::get_closest_point_line(ic::Vec2f &point, ic::Vec2f sidePointA, ic::Vec2f sidePointB) {
    ic::Vec2f gradientLine = sidePointB - sidePointA;
    ic::Vec2f gradientPoint = point - sidePointA;
    float t = gradientLine.dot(gradientPoint) / gradientLine.len2();
    t = ic::Mathf::clamp(t, 0.0f, 1.0f);
    return sidePointA + gradientLine * t;
}


ic::RaycastHit<3> ic::Raycast::ray_triangle(ic::Vec3 p1, ic::Vec3 p2, ic::Vec3 p3, const ic::Vec3 &rayOrigin, const ic::Vec3 &rayDir) {
    
}