#ifndef IC_ORBITAL_CAMERA_CONTROLLER_3D_H
#define IC_ORBITAL_CAMERA_CONTROLLER_3D_H

#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/input/MouseController.h>
#include <Icosahedron/input/InputHandler.h>

#include <Icosahedron/scene/3d/Camera3D.h>

namespace ic {
    class OrbitalCameraController3D {
        public:
            ic::Vec3f center;
            ic::Vec2f rotation;
            
            float radius;
            float lookSensitivity;

            OrbitalCameraController3D(ic::Camera3D *camera, ic::InputHandler *handler);

            void act(float dt);

        protected:
            ic::Camera3D *camera;
            ic::MouseController *mouse;
    };
}
#endif