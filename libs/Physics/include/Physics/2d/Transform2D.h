#ifndef IC_PHYSICS_2D_TRANSFORM_H
#define IC_PHYSICS_2D_TRANSFORM_H

#include <Icosahedron/math/geom/Vectors.h>

namespace ic { namespace Physics {
    /** @brief Stores information about an object's location in 2D. */
    struct Transform2D {
        ic::Vec2f position;
        ic::Vec2f scale;
        float angle;

        Transform2D();
    };
}}

#endif