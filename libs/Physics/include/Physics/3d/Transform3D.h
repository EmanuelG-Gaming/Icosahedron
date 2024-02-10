#ifndef IC_PHYSICS_3D_TRANSFORM_H
#define IC_PHYSICS_3D_TRANSFORM_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/geom/Quaternion.h>

namespace ic { namespace Physics {
    /** @brief Stores information about an object's location in 3D. */
    struct Transform3D {
        ic::Vec3f position;
        ic::Vec3f scale;
        ic::Quaternion rotation;

        Transform3D();
    };
}}

#endif