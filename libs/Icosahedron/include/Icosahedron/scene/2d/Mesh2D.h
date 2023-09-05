#ifndef IC_MESH_2D
#define IC_MESH_2D

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
    struct MeshMaterial2D {
        float colorBlending;
        ic::Vec3f baseColor;

        MeshMaterial2D() : baseColor({1.0f, 1.0f, 1.0f}), colorBlending(1.0f) {}
        MeshMaterial2D(ic::Color baseColor, float colorBlending = 1.0f) : baseColor({ baseColor.r / 255.0f, baseColor.g / 255.0f, baseColor.b / 255.0f }), colorBlending(colorBlending) {}
    };

    class Mesh2D {
        public:
            /** @brief Constructs a new, but empty mesh. */
            Mesh2D();
            /** @brief Constructs a mesh that has a position vertex attribute. */
            Mesh2D(std::vector<float> vertexPositions);

            /** @brief Overrides the mesh's current model-level transformation matrix. */
            void set_transformation(const ic::Mat4x4 &to);

            void set_material(ic::MeshMaterial2D newMaterial);

            void add_attribute(const std::string &location, int dimensions, const std::vector<float> &content);
            void add_attribute(const std::string &location, int dimensions, const std::vector<ic::Color> &content);
            void set_index_buffer(const std::vector<unsigned int> &content);
            
            void jump_attribute();
            
            ic::GLAttribute &get_attribute(const std::string &location);

            void unuse_attribute_definitions();

            void draw(ic::Shader *shader);
            void dispose();

        protected:
            void upload_material(ic::Shader *shader, const ic::MeshMaterial2D &mat);

        private:
            std::map<std::string, ic::GLAttribute> vertexAttributes;
            std::vector<unsigned int> indices;

            ic::VertexArray *vao = nullptr;
            ic::MeshMaterial2D material;

            ic::Mat4x4 model;
    };
}
#endif