#ifndef IC_PHYSICS_MANIFOLD_POINTS_2D_H
#define IC_PHYSICS_MANIFOLD_POINTS_2D_H

#include <Icosahedron/math/geom/Vectors.h>

namespace ic { namespace Physics {
    struct ManifoldPoints2D {
        /** @brief The furthest point from object A to object B. */
        ic::Vec2f AtoB;

        /** @brief The furthest point from object B to object A. */
        ic::Vec2f BtoA;

        /** @brief BtoA - AtoB, normalized. */
        ic::Vec2f normal;

        /** @brief Length of the full normal vector. */
        float depth = 0.0f;

        /** @brief Whether or not the objects have collided. */
        bool collided = false;

        ManifoldPoints2D() {}
        ManifoldPoints2D(const ic::Vec2f &AtoB, const ic::Vec2f &BtoA, const ic::Vec2f &normal, float depth) :
            AtoB(AtoB), BtoA(BtoA), normal(normal), depth(depth), collided(true) {}
    };
}}

#endif