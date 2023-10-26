#ifndef IC_CAMERA_2D_H
#define IC_CAMERA_2D_H

#include <Icosahedron/Global.h>

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/Matrices.h>

#include <Icosahedron/graphics/gl/Shader.h>


namespace ic {
    /** @brief Represents a camera in a two-dimensional space. */
    class Camera2D {
        public:
            ic::Vec2f position;
            float scale;
            bool correctsAspectRatio;
            int width, height;
            
            Camera2D();
            Camera2D(ic::Vec2f position, float scale);
            Camera2D(float scale);

            /* Calculates and sends the camera's projection matrix to a shader program. */
            void use(ic::Shader *shader);
            void unuse(ic::Shader *shader);

            /* Projects world coordinates to screen coordinates. */
            ic::Vec2f project(ic::Vec2f &worldPosition);

            /* Projects screen coordinates to world coordinates. */
            ic::Vec2f unproject(ic::Vec2f &screenPosition);
        
        protected:
            ic::Mat4x4 projection;
            ic::Mat4x4 aspectRatioCorrection;
    };
}
#endif