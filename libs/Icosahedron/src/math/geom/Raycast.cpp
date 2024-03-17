#include <Icosahedron/math/geom/Raycast.h>

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
    bool intersects = (t >= 0.0f && t <= 1.0f) && (u >= 0.0f && u <= 1.0f);
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
        result.hitNormals[0] = (result.hitPos[0] - sphereCenter).nor();
        result.hitNormals[1] = (result.hitPos[1] - sphereCenter).nor();

        result.collided = true;

        return result;
    }

    result.collided = false;
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