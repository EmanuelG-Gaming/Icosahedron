#ifndef IC_CAMERA_3D_H
#define IC_CAMERA_3D_H

#include <string>

#include <Icosahedron/Global.h>
#include <Icosahedron/graphics/gl/Shader.h>

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/Matrices.h>


namespace ic {
    /** @brief Settings for a Camera3D. */
    struct CameraSettings {
        /** The field of view of this camera, in degrees. */
        float fov = 90.0f;

        /** The near and the far clipping planes. */
        float zNear = 0.1f, zFar = 1000.0f;

        /** The viewport's size. */
        float width = float(IC_WINDOW_WIDTH), height = float(IC_WINDOW_HEIGHT);

        /** Is perspective projection used here?
         *  If not, orthographic projection will be used instead. */
        bool perspective = true;
    };

    /** @brief Represents a camera in a three-dimensional space. */
    class Camera3D {
        public:
            CameraSettings settings;
            bool correctsAspectRatio;
            
            ic::Vec3f position;
            ic::Vec3f up;
            ic::Vec3f lookingAt;
            
            /** Initializes a new camera that points in the X direction. */
            Camera3D();
            Camera3D(float fov, float zNear, float zFar, float width, float height, bool perspective);
            Camera3D(bool perspective);

            void update();
            void upload_to_shader(ic::Shader *shader, const std::string &viewUniform = "view", const std::string &projectionUniform = "projection");

            ic::Mat4x4 &get_view();
            ic::Mat4x4 &get_projection();

            void resize(float width, float height);

            /** @brief Returns a vector pointing from the camera's origin to a location on the camera's screen plane. */
            ic::Vec3f unproject(ic::Vec2f &screenPosition);

        protected:
            ic::Mat4x4 viewMatrix, projectionMatrix, aspectRatioCorrection;
        
            /** The projection matrix multiplied by the view matrix. */
            ic::Mat4x4 combined;
    };
}
#endif