#ifndef IC_MESH_3D_H
#define IC_MESH_3D_H

#include <vector>
#include <string>
#include <map>

#include <Icosahedron/math/Matrices.h>
#include <Icosahedron/graphics/Color.h>

#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/VertexArray.h>



namespace ic {
    class Mesh3D {
        public:
            /** @brief Constructs a new, but empty mesh. */
            Mesh3D();
            /** @brief Constructs a mesh that has a position vertex attribute. */
            Mesh3D(std::vector<float> vertexPositions);

            /** @brief Overrides the mesh's current model-level transformation matrix. */
            void set_transformation(const ic::Mat4x4 &to);

            void add_attribute(const std::string &location, int dimensions, const std::vector<float> &content);
            void add_attribute(const std::string &location, int dimensions, const std::vector<ic::Color> &content);
            void set_index_buffer(const std::vector<unsigned int> &content);
            
            void jump_attribute();
            
            ic::GLAttribute &get_attribute(const std::string &location);

            void unuse_attribute_definitions();

            void draw(ic::Shader *shader);
            void dispose();

        private:
            std::map<std::string, ic::GLAttribute> vertexAttributes;
            std::vector<unsigned int> indices;

            ic::VertexArray *vao = nullptr;
            
            ic::Mat4x4 model;
    };
}
#endif