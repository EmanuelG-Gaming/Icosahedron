#ifndef IC_PHYSICS_MANIFOLD_POINTS_H
#define IC_PHYSICS_MANIFOLD_POINTS_H

#include <Icosahedron/math/geom/Vectors.h>


namespace ic { namespace Physics {
    struct ManifoldPoints {
        /** @brief The furthest point from object A to object B. */
        ic::Vec3f AtoB;

        /** @brief The furthest point from object B to object A. */
        ic::Vec3f BtoA;

        /** @brief BtoA - AtoB, normalized. */
        ic::Vec3f normal;

        /** @brief Length of the full normal vector. */
        float depth = 0.0f;

        /** @brief Whether or not the objects have collided. */
        bool collided = false;

        ManifoldPoints() {}
        ManifoldPoints(const ic::Vec3f &AtoB, const ic::Vec3f &BtoA, const ic::Vec3f &normal, float depth) :
            AtoB(AtoB), BtoA(BtoA), normal(normal), depth(depth), collided(true) {}
        
        ManifoldPoints(const ic::Vec3f &normal, float depth) : normal(normal), depth(depth), collided(true) {}
    };
}}

#endif