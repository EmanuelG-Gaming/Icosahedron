#ifndef IC_PHYSICS_TRANSFORM_H
#define IC_PHYSICS_TRANSFORM_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/geom/Quaternion.h>

namespace ic { namespace Physics {
    /** @brief Stores information about an object's location in 3D. */
    struct Transform {
        ic::Vec3 position;
        ic::Vec3 scale;
        ic::Quaternion rotation;

        Transform();
    };
}}

#endif