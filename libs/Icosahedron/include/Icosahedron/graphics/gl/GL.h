#ifndef IC_GL_H
#define IC_GL_H

#include <vector>

#include <GL/glew.h>

#include <Icosahedron/math/geom/Vector.h>

namespace ic {
    enum GLPrimitives {
        INVALID_PRIMITIVE = -1,

        POINTS = GL_POINTS,
        LINES = GL_LINES,
        TRIANGLES = GL_TRIANGLES,
        TRIANGLE_FANS = GL_TRIANGLE_FAN,
        TRIANGLE_STRIPS = GL_TRIANGLE_STRIP,
    };

    enum GLTextureTypes {
        INVALID_TEXTURE = -1,

        T2D = GL_TEXTURE_2D,
        TCUBE = GL_TEXTURE_CUBE_MAP,
    };

    enum GLTextureAttachments {
        INVALID_TEXTURE_ATTACHMENT = -1,

        TEXTURE_ATTACH_COLOR_0 = GL_COLOR_ATTACHMENT0
    };

    enum GLDepthSettings {
        INVALID_DEPTH_SETTING = -1,

        LESS = GL_LESS,
        GREATER = GL_GREATER,
        EQUAL = GL_EQUAL,

        ALWAYS = GL_ALWAYS,
        NEVER = GL_NEVER,
    };

    enum GLCullSettings {
        INVALID_CULL_SETTING = -1,

        FRONT = GL_FRONT,
        BACK = GL_BACK,
    };

    enum GLFaceWindingOrder {
        INVALID_FACE_WINDING = -1,

        CW = GL_CW,
        CCW = GL_CCW, // The default winding order of OpenGL.
    };

    enum GLBlendingSource {
        INVALID_BLEND_SOURCE = -1,

        SRC_ZERO = GL_ZERO,
        SRC_ONE = GL_ONE,

        SRC_DEST_COLOR = GL_DST_COLOR,
        SRC_ONE_MINUS_DEST_COLOR = GL_ONE_MINUS_DST_COLOR,
        SRC_ALPHA = GL_SRC_ALPHA,
        SRC_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA
    };
    
    enum GLBlendingDestination {
        INVALID_BLEND_DEST = -1,

        DEST_ZERO = GL_ZERO,
        DEST_ONE = GL_ONE,

        DEST_SRC_COLOR = GL_SRC_COLOR,
        DEST_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
        DEST_SRC_ALPHA = GL_SRC_ALPHA,
        DEST_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
    };

    enum GLContextSettings {
        INVALID_GL_CONTEXT_SETTING = -1,

        DEPTH_TESTING = GL_DEPTH_TEST,
        BLENDING = GL_BLEND,
        FACE_CULLING = GL_CULL_FACE
    };


    struct GLAttribute {
        std::vector<float> content;
        int dimensions;
    };
}

#endif