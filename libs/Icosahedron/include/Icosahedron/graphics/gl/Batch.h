#ifndef IC_BATCH_H
#define IC_BATCH_H

#include <vector>

#include <glad/glad.h>

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/graphics/gl/GL.h>

namespace ic {
    /* A vertex for a batch. */
    struct BatchVertex {
        ic::Vec2f Position;
        ic::Vec4f Color;
        ic::Vec2f TextureCoords;

        BatchVertex() {}
        BatchVertex(float x, float y, ic::Color color) : Position({x, y}), Color({color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,  color.a / 255.0f }) {}
        BatchVertex(float x, float y, float tx, float ty, ic::Color color) : Position({x, y}), TextureCoords({tx, ty}), Color({color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f}) {}
    };

    /*
    enum BatchVertexAttributeDataType {
        BATCH_FLOAT,
        BATCH_INT
    };

    struct BatchVertexAttribute {
        ic::BatchVertexAttributeDataType type;
        std::size_t dimensions;
    };

    struct BatchVertexLayout {
        std::vector<ic::BatchVertexAttribute> attributes;
    };
    */
    
    class Batch {
        public:
            Batch();
            Batch(int capacity, ic::GLPrimitives renderType);

            void add(const std::vector<BatchVertex> &vertices);
            
            //void add_attribute(int attributeIndex, int dimensions);
            
            void render();
            void dispose();

            int get_extra_vertices();

        private:
            void setup();

        protected:
            std::vector<GLuint> bufferObjects;
            std::vector<int> attributeDimensions;

            int vertexCapacity;
            int verticesUsed;
            BatchVertex lastUsed;
            GLPrimitives renderType;

            GLuint vbo;
            GLuint vao;
    };
}

#endif