#ifndef IC_CAMERA_3D_H
#define IC_CAMERA_3D_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/Matrices.h>


namespace ic {
    /** @brief Represents a camera in a three-dimensional space. */
    class Camera3D {
        public:
            ic::Vec3f position;
            ic::Vec3f lookingAt;
            ic::Vec3f lookDirection;
            float rotationX, rotationY;
            
    };
}
#endif