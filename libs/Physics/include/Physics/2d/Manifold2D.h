#ifndef IC_PHYSICS_MANIFOLD_2D_H
#define IC_PHYSICS_MANIFOLD_2D_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Physics/2d/objects/Object2D.h>
#include <Physics/2d/ManifoldPoints2D.h>

namespace ic { namespace Physics {
    struct Manifold2D {
        Object2D *object1, *object2;
        ManifoldPoints2D points;

        Manifold2D(Object2D *object1, Object2D *object2, ManifoldPoints2D points) :
            object1(object1), object2(object2), points(points) {}
    };
}}

#endif