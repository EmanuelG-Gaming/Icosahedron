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
        float x, y;
        float r, g, b;
        float tx, ty;

        BatchVertex() {}
        BatchVertex(float x, float y, ic::Color color) : x(x), y(y), r(color.r / 255.0f), g(color.g / 255.0f), b(color.b / 255.0f) {}
        BatchVertex(float x, float y, float tx, float ty, ic::Color color) : x(x), y(y), tx(tx), ty(ty), r(color.r / 255.0f), g(color.g / 255.0f), b(color.b / 255.0f) {}
    };
    
    struct BatchVertexBuffer {
        int dimensions;

        // The amount of components of each vertex used by the vertex buffer
        int verticesUsed = 0;
        float lastUsed = 0;
        
        // The VBO itself
        GLuint vbo = 0;

        BatchVertexBuffer(int dimensions, GLuint vbo) : dimensions(dimensions), vbo(vbo) {}
    };

    class Batch2D {
        public:
            Batch2D(int capacity, ic::GLPrimitives renderType);
            
            /** @brief Adds data to one of the attributes (more specifically, to one of the VBOs).
             *  @param vertexBufferIndex Index to locate the vertex buffer object.
             *  @param vertices The vertex data to append to the batch pool. This may not be the full data, though, as vertices can have more than 1 attribute.
            */
            void add_vertex_data(int vertexBufferIndex, std::vector<float> &vertices);

            void add_vertex_data(int vertexBufferIndex, std::vector<ic::Color> &colors);
            
            /** @brief Adds indices to this batch. */
            void add_indices(std::vector<unsigned int> &indices);

            void render();
            void dispose();

            /** @brief Jumps to the next attribute, based on the location
             *  inside the shader program, without adding any vertex buffer.
            */
            void jump();

            /** @brief Adds an empty floating-point vertex buffer to this batch.
             *  @param dimensions The number of dimensions of the attribute vector.
            */
            void add_vertex_buffer(int dimensions);
            
            void use();
            void unuse();
            void unuse_attribute_definitions();
            
            int get_extra_vertices();
            
        private:
            void setup();
            void reset();
            void set_index_buffer();

        protected:
            int vertexCapacity;
            GLPrimitives renderType;

            std::vector<BatchVertexBuffer> bufferObjects;
            std::vector<unsigned int> storedIndices;
            
            unsigned int currentIndexOffset = 0;

            GLuint vao = 0, ibo = 0;
            int lastVertexBufferPosition = 0;
            int indicesUsed = 0;
            
    };
}

#endif