#ifndef IC_MESH_H
#define IC_MESH_H

#include <vector>
#include <map>
#include <string>

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/Matrices.h>

#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/VertexArray.h>
#include <Icosahedron/graphics/Color.h>


namespace ic {
    class Mesh {
        public:
            /** @brief Constructs a new, but empty mesh. */
            Mesh();
            
            /** @brief Constructs a new mesh, depending on a vertex array. */
            Mesh(ic::VertexArray &vao);


            /** @brief Overrides the mesh's current model-level transformation matrix. */
            void set_transformation(const ic::Mat4x4 &to);

            /** @brief Incrementally multiplies this mesh's model matrix with another matrix. */
            void combine_transformation(ic::Mat4x4 &with);

            void set_normal_transformation(const ic::Mat4x4 &to);
            void combine_normal_transformation(ic::Mat4x4 &with);
            
            
            void attribute(int attributeIndex, int dimensions, const std::vector<float> &content);

            void add_attribute(int attributeIndex, int dimensions, const std::vector<float> &content);
            void add_attribute(int attributeIndex, int dimensions, const std::vector<int> &content);
            void add_attribute(int attributeIndex, int dimensions, const std::vector<ic::Color> &content);
            void set_index_buffer(const std::vector<unsigned int> &content);
            
            void unuse_attribute_definitions();

            void draw(ic::GLPrimitives primitive = ic::TRIANGLES);
            void apply_transformations(ic::Shader &shader, const char *modelMatName = "model", const char *normalModelMatName = "normalModel");

            void dispose();

            void using_indices(bool to);
            void set_index_count(GLsizei to);
            

        private:
            ic::VertexArray vao;
            
            ic::Mat4x4 model, normalModel;
    };
}
#endif