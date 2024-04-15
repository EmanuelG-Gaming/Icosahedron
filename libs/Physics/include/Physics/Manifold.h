#ifndef IC_PHYSICS_MANIFOLD_H
#define IC_PHYSICS_MANIFOLD_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Physics/objects/Object.h>

namespace ic { namespace Physics {
    struct Manifold {
        Object *object1, *object2;
        ManifoldPoints points;

        Manifold(Object *object1, Object *object2, ManifoldPoints points) :
            object1(object1), object2(object2), points(points) {}
    };
}}

#endif