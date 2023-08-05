#ifndef IC_GL_H
#define IC_GL_H

#include <initializer_list>

#include <GL/glew.h>

#include <Icosahedron/math/geom/Vector.h>

namespace ic {
    enum GLPrimitives {
        INVALID_PRIMITIVE = -1,

        POINTS = GL_POINTS,
        LINES = GL_LINES,
        TRIANGLES = GL_TRIANGLES,
        TRIANGLE_FANS = GL_TRIANGLE_FAN,
        TRIANGLE_STRIPS = GL_TRIANGLE_STRIP
    };

    enum GLTextureTypes {
        T2D = GL_TEXTURE_2D,
        TCube = GL_TEXTURE_CUBE_MAP,
    };
}

#endif