#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/Global.h>

#include <stdexcept>


using namespace ic;

Mesh2D::Mesh2D() {
    if (IC_IS_OPENGL_CONTEXT_PRESENT) {
        this->vao.setup();
    }
}

Mesh2D::Mesh2D(ic::VertexArray &vao) {
    if (IC_IS_OPENGL_CONTEXT_PRESENT) {
        this->vao = vao;
    }
}

void ic::Mesh2D::set_material(ic::MeshMaterial2D newMaterial) {
    this->material = newMaterial;
}

void ic::Mesh2D::set_transformation(const ic::Mat4x4 &to) {
    this->model = to;
}

void ic::Mesh2D::combine_transformation(ic::Mat4x4 &with) {
    this->model = this->model * with;
}

void ic::Mesh2D::attribute(int attributeIndex, int dimensions, const std::vector<float> &content) {
    this->vao.vertex_buffer(attributeIndex, dimensions, content);
}


void ic::Mesh2D::add_attribute(int attributeIndex, int dimensions, const std::vector<float> &content) {
    this->vao.add_vertex_buffer(attributeIndex, dimensions, content);
}
void ic::Mesh2D::add_attribute(int attributeIndex, int dimensions, const std::vector<int> &content) {
    this->vao.add_vertex_buffer(attributeIndex, dimensions, content);
}


void ic::Mesh2D::add_attribute(int attributeIndex, int dimensions, const std::vector<ic::Color> &content) {
    std::vector<float> colorValues;
    for (int i = 0; i < content.size(); i++) {
        ic::Color color = content[i];

        colorValues.push_back(color.r / 255.0f);
        colorValues.push_back(color.g / 255.0f);
        colorValues.push_back(color.b / 255.0f);
    }
    add_attribute(attributeIndex, dimensions, colorValues);
}




void ic::Mesh2D::set_index_buffer(const std::vector<unsigned int> &content) {
    this->vao.set_index_buffer(content);
}

void ic::Mesh2D::unuse_attribute_definitions() {
    this->vao.unuse_attribute_definitions();
}

void ic::Mesh2D::draw(ic::Shader &shader, ic::GLPrimitives primitive) {
    auto drawable = this->vao.get_drawable();

    this->upload_material(shader, this->material);
    shader.set_uniform_mat4("model", this->model);
    drawable.use_and_draw(primitive);
}

void ic::Mesh2D::dispose() {
    this->vao.dispose();
}

void ic::Mesh2D::upload_material(ic::Shader &shader, const ic::MeshMaterial2D &mat) {
    shader.set_uniform_float("material.colorBlending", mat.colorBlending);
    shader.set_uniform_vec3f("material.baseColor", mat.baseColor);
}