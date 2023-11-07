#ifndef IC_GL_H
#define IC_GL_H

#include <vector>

#include <glad/glad.h>

#include <Icosahedron/math/geom/Vector.h>

namespace ic {
    enum GLPrimitives {
        INVALID_PRIMITIVE = -1,

        POINTS = GL_POINTS,
        LINES = GL_LINES,
        LINE_STRIPS = GL_LINE_STRIP,

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

        TEXTURE_ATTACH_COLOR_0 = GL_COLOR_ATTACHMENT0,
        TEXTURE_ATTACH_COLOR_1 = GL_COLOR_ATTACHMENT1,
        TEXTURE_ATTACH_COLOR_2 = GL_COLOR_ATTACHMENT2,
        TEXTURE_ATTACH_COLOR_3 = GL_COLOR_ATTACHMENT3,
        
        TEXTURE_ATTACH_DEPTH = GL_DEPTH_ATTACHMENT,
    };

    enum GLTextureColorChannels {
        INVALID_TEXTURE_COLOR_CHANNEL = -1,

        TEXTURE_RGB = GL_RGB,
        TEXTURE_RGBA = GL_RGBA,
        TEXTURE_RGBA_16F = GL_RGBA16F,
        TEXTURE_RGBA_32F = GL_RGBA32F,
        TEXTURE_DEPTH = GL_DEPTH_COMPONENT,
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

    enum GLTextureFilter {
        INVALID_TEXTURE_FILTER = -1,

        TEXTURE_FILTER_NEAREST = GL_NEAREST,
        TEXTURE_FILTER_LINEAR = GL_LINEAR,
    };

    enum GLTextureWrap {
        INVALID_TEXTURE_WRAP = -1,

        TEXTURE_WRAP_REPEAT = GL_REPEAT,
        TEXTURE_WRAP_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        TEXTURE_WRAP_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        TEXTURE_WRAP_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    };
}

#endif