#ifndef IC_VERTEX_ARRAY_H
#define IC_VERTEX_ARRAY_H

#include <vector>

#include <GL/glew.h>

#include <Icosahedron/graphics/gl/GL.h>


namespace ic {
    /** @brief Drawn using a vertex array object with glDrawElements. */
    class VertexArrayDrawable final {
        public:
            VertexArrayDrawable(GLuint vao, GLsizei indicesUsed);

            void use();
            void draw(GLPrimitives primitive = TRIANGLES);

            void use_and_draw(GLPrimitives primitive = TRIANGLES);

        private:
            const GLuint vao = 0;
            const GLsizei indicesUsed = 0;
    };

    /** @brief A wrapper for an OpenGL vertex array object (VAO). 
     * This can have multiple vertex buffers, depending on what
     * attributes are being used. */
    class VertexArray {
        public:
            VertexArray();

            VertexArrayDrawable get_drawable();

            void use();
            void unuse();

            void dispose();

            /** @brief Adds a floating-point vertex buffer to this VAO.
             *  @param dimensions The number of dimensions of the attribute vector.
             *  @param content Attribute content in the following form: { x1, y1, ... n1,
             *                                                            x2, y2,  ... n2,
             *                                                            ... }, where n is the number of dimensions.
            */
            void add_vertex_buffer(int dimensions, const std::vector<GLfloat> &content);
            void add_index_buffer(const std::vector<GLuint> &content);

            void unuse_attribute_definitions();

        protected:
            void setup();

        private:
            std::vector<GLuint> bufferObjects;
            GLuint vao = 0;
            GLsizei indicesUsed = 0;

            int indexBuffersAdded = 0;
    };
}

#endif