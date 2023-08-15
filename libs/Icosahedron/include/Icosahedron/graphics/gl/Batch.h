#ifndef IC_BATCH_H
#define IC_BATCH_H

#include <vector>

#include <GL/glew.h>

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/graphics/gl/GL.h>

namespace ic {
    /* @brief A vertex for a Batch2D. */
    struct BatchVertex {
        float x, y, z;
        float r, g, b;
        float tx, ty;

        BatchVertex() {}
        BatchVertex(float x, float y, ic::Color color) : x(x), y(y), z(z), r(color.r / 255.0f), g(color.g / 255.0f), b(color.b / 255.0f) {}
        BatchVertex(float x, float y, float tx, float ty, ic::Color color) : x(x), y(y), tx(tx), ty(ty), r(color.r / 255.0f), g(color.g / 255.0f), b(color.b / 255.0f) {}
    };
    
    class Batch2D {
        public:
            Batch2D(int capacity, ic::GLPrimitives renderType);
            
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
    };
}

#endif