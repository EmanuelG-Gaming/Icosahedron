#ifndef IC_BATCH_H
#define IC_BATCH_H

#include <vector>

#include <GL/glew.h>

#include <Icosahedron/math/geom/Vector.h>
#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/graphics/gl/Shader.h>

enum GLPrimitives {
    INVALID_PRIMITIVE = 0,

    POINTS = GL_POINTS,
    LINES = GL_LINES,
    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_FANS = GL_TRIANGLE_FAN,
    TRIANGLE_STRIPS = GL_TRIANGLE_STRIP
};


namespace ic {
    /* A vertex for a Batch2D. */
    struct BatchVertex {
        ic::Vector<float, 2> Position;
        ic::Vector<float, 3> Color;

        BatchVertex() {}
        BatchVertex(float x, float y, ic::Color color) : Position({x, y}), Color({color.r / 255.0f, color.g / 255.0f, color.b / 255.0f}) {}
    };
    
    class Batch2D {
        public:
            Batch2D(int capacity, GLPrimitives renderType);
            
            void add(const std::vector<ic::BatchVertex> &vertices);
            void render();
            void dispose();

            int get_extra_vertices();

        private:
            void setup();

        protected:
            int vertexCapacity;
            int verticesUsed;
            BatchVertex lastUsed;
            GLPrimitives renderType;

            GLuint vbo;
            GLuint vao;
            ic::Shader *shader;
    };
}

#endif