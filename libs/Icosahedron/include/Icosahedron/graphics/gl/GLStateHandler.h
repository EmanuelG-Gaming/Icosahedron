#ifndef IC_GL_CONTEXT_H
#define IC_GL_CONTEXT_H

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <Icosahedron/graphics/gl/GL.h>

namespace ic {
    class GLStateHandler {
        public:
            void enable_depth_testing(ic::GLDepthSettings depth);
            void enable_blending(ic::GLBlendingSource source, ic::GLBlendingDestination dest);
            void enable_face_culling(ic::GLCullSettings culling, ic::GLFaceWindingOrder winding = ic::CCW);

            void set_depth_testing(ic::GLDepthSettings depth);
            void set_blending(ic::GLBlendingSource source, ic::GLBlendingDestination dest);
            void set_face_culling(ic::GLCullSettings culling, ic::GLFaceWindingOrder winding = ic::CCW);

            void set_viewport(int w, int h);
            
            void enable(ic::GLContextSettings setting);
            void disable(ic::GLContextSettings setting);

            void disable_depth_testing();
            void disable_blending();
            void disable_face_culling();
    };
}
#endif