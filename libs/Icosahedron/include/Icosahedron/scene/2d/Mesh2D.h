#ifndef IC_MESH_2D
#define IC_MESH_2D

#include <vector>
#include <map>
#include <string>

#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/VertexArray.h>

namespace ic {
    class Mesh2D {
        public:
            /** @brief Constructs a new, but empty mesh. */
            Mesh2D();
            /** @brief Constructs a mesh that has a position vertex attribute. */
            Mesh2D(std::vector<float> vertexPositions);

            void add_attribute(const std::string &location, int dimensions, const std::vector<float> &content);
            void add_index_buffer(const std::vector<unsigned int> &content);
            
            ic::GLAttribute &get_attribute(const std::string &location);

            void unuse_attribute_definitions();

            void draw();

        private:
            std::map<std::string, ic::GLAttribute> vertexAttributes;
            std::vector<unsigned int> indices;

            ic::VertexArray *vao = nullptr;
    };
}
#endif