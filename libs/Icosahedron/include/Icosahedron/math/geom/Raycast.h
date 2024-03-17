#ifndef IC_RAYCAST_H
#define IC_RAYCAST_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/Mathf.h>

#define IC_RAYCAST_MAX_HITS 5


namespace ic {
    /** @brief The hit positions are ought to be sorted by the distance from the ray's origin to the hit position,
     *  meaning that the closest distance percentage will be at the index 0.
    */ 
    template <std::size_t dims>
    struct RaycastHit {
        float hitAlphas[IC_RAYCAST_MAX_HITS];
        ic::Vector<float, dims> hitPos[IC_RAYCAST_MAX_HITS];
        ic::Vector<float, dims> hitNormals[IC_RAYCAST_MAX_HITS];
        
        ic::Vector<float, dims> rayPos, rayDir;
        bool collided = false;
    };

    /** @brief A suite of utilities made for testing collisions between various objects and rays and also
     *  getting the closest point to a shape.
    */
    namespace Raycast {
        bool line_segments_collide(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

        template <std::size_t dims>
        ic::RaycastHit<dims> ray_nsphere(ic::Vector<float, dims> sphereCenter, float radius, const ic::Vector<float, dims> &rayOrigin, const ic::Vector<float, dims> &rayDir);


        ic::RaycastHit<3> ray_sphere(ic::Vec3 sphereCenter, float radius, const ic::Vec3 &rayOrigin, const ic::Vec3 &rayDir);
        
        ic::Vec2f get_closest_point_line(ic::Vec2f &point, ic::Vec2f sidePointA, ic::Vec2f sidePointB);
    };
}

#endif