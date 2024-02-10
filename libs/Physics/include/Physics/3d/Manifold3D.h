#ifndef IC_PHYSICS_MANIFOLD_3D_H
#define IC_PHYSICS_MANIFOLD_3D_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Physics/3d/objects/Object3D.h>
#include <Physics/3d/ManifoldPoints3D.h>

namespace ic { namespace Physics {
    struct Manifold3D {
        Object3D *object1, *object2;
        ManifoldPoints3D points;

        Manifold3D(Object3D *object1, Object3D *object2, ManifoldPoints3D points) :
            object1(object1), object2(object2), points(points) {}
    };
}}

#endif