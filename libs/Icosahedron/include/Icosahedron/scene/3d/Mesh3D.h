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
            
            /** @brief Constructs a new mesh based on a vertex array. */
            Mesh3D(ic::VertexArray &vao);


            /** @brief Overrides the mesh's current model-level transformation matrix. */
            void set_transformation(const ic::Mat4x4 &to);

            /** @brief Incrementally multiplies this mesh's model matrix with another matrix. */
            void combine_transformation(ic::Mat4x4 &with);

            /** @brief Overrides the mesh normals' current model-level transformation matrix. */
            void set_normal_transformation(const ic::Mat4x4 &to);

            void combine_normal_transformation(ic::Mat4x4 &with);


            void add_attribute(int attributeIndex, int dimensions, const std::vector<float> &content);
            void add_attribute(int attributeIndex, int dimensions, const std::vector<int> &content);
            void add_attribute(int attributeIndex, int dimensions, const std::vector<ic::Color> &content);
            void add_attribute(int attributeIndex, const std::vector<ic::Vec2f> &content);
            void add_attribute(int attributeIndex, const std::vector<ic::Vec3f> &content);

            void set_index_buffer(const std::vector<unsigned int> &content);
            
            void unuse_attribute_definitions();

            void draw(ic::Shader &shader, ic::GLPrimitives primitive = ic::TRIANGLES);
            void dispose();

            void using_indices(bool to);
            void set_index_count(GLsizei to);

        private:
            std::vector<unsigned int> indices;

            ic::VertexArray vao;
            
            ic::Mat4x4 model;
            ic::Mat4x4 normalModel;
    };
}
#endif