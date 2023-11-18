#ifndef IC_FREE_ROAM_CAMERA_CONTROLLER_3D
#define IC_FREE_ROAM_CAMERA_CONTROLLER_3D

#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/input/KeyboardController.h>
#include <Icosahedron/input/MouseController.h>
#include <Icosahedron/input/InputHandler.h>

#include <Icosahedron/scene/3d/Camera3D.h>


namespace ic {
    class FreeRoamCameraController3D {
        public:
            ic::Vec2f rotation;
            
            bool flying;
            float speed, lookSensitivity;

            FreeRoamCameraController3D();
            FreeRoamCameraController3D(ic::Camera3D *camera);

            void act(float dt);

        protected:
            ic::Camera3D *camera;

            ic::KeyboardController *keyboard;
            ic::MouseController *mouse;
    };
}
#endif