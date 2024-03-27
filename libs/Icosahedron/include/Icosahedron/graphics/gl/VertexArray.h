#ifndef IC_VERTEX_ARRAY_H
#define IC_VERTEX_ARRAY_H

#include <vector>

#include <glad/glad.h>

#include <Icosahedron/graphics/gl/GL.h>


namespace ic {
    /** @brief Drawn using a vertex array object with glDrawElements. */
    class VertexArrayDrawable final {
        public:
            VertexArrayDrawable(GLuint vao, GLuint ibo, GLsizei indicesUsed, bool usingIndices);

            void use();
            void draw(GLPrimitives primitive = TRIANGLES);

            void use_and_draw(GLPrimitives primitive = TRIANGLES);

        private:
            const GLuint vao = 0;
            const GLuint ibo = 0;
            const GLsizei indicesUsed = 0;
            const bool usingIndices = false;
    };

    /** @brief A wrapper for an OpenGL vertex array object (VAO). 
     * This can have multiple vertex buffers, depending on what
     * attributes are being used. 
     * 
     * By default, the VAO uses an index buffer. */
    class VertexArray {
        public:
            VertexArray();

            VertexArrayDrawable get_drawable();

            void setup();
            
            
            void use();
            void unuse();

            void dispose();

            /** @brief Sets a floating-point vertex buffer to this VAO, without appending it to a CPU-side buffer object pool,
             *  making it useful for dynamically changing animation frames.
             *  @param attributeIndex The index of the attribute, as in the shader code.
             *  @param dimensions The number of dimensions of the attribute vector.
             *  @param content Attribute content in the following form: { x1, y1, ... n1,
             *                                                            x2, y2,  ... n2,
             *                                                            ... }, where n is the number of dimensions.
            */
            GLuint vertex_buffer(int attributeIndex, int dimensions, const std::vector<float> &content);

            /** @brief Sets an integer vertex buffer to this VAO, without appending it to a CPU-side buffer object pool,
             *  making it useful for dynamically changing animation frames.
             *  @param attributeIndex The index of the attribute, as in the shader code.
             *  @param dimensions The number of dimensions of the attribute vector.
             *  @param content Attribute content in the following form: { x1, y1, ... n1,
             *                                                            x2, y2,  ... n2,
             *                                                            ... }, where n is the number of dimensions.
            */
            GLuint vertex_buffer(int attributeIndex, int dimensions, const std::vector<int> &content);

            GLuint add_vertex_buffer(int attributeIndex, int dimensions, const std::vector<float> &content);
            GLuint add_vertex_buffer(int attributeIndex, int dimensions, const std::vector<int> &content);


            GLuint set_index_buffer(const std::vector<unsigned int> &content);

            void unuse_attribute_definitions();

            ic::VertexArray &using_indices(bool to);
            ic::VertexArray &set_index_count(GLsizei to);

        protected:
            void reset();
            
        private:
            std::vector<GLuint> bufferObjects;
            GLuint vao = 0, ibo = 0;
            GLsizei indicesUsed = 0;
            bool usingIndices = true;
    };
}

#endif