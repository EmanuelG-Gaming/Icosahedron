#include <Icosahedron/scene/Mesh.h>
#include <Icosahedron/Global.h>

#include <stdexcept>


using namespace ic;

Mesh::Mesh() {
    if (IC_IS_OPENGL_CONTEXT_PRESENT) {
        this->vao.setup();
    }
}

Mesh::Mesh(ic::VertexArray &vao) {
    this->vao = vao;
}


void ic::Mesh::set_transformation(const ic::Mat4x4 &to) {
    this->model = to;
}

void ic::Mesh::combine_transformation(ic::Mat4x4 &with) {
    this->model = this->model * with;
}

void ic::Mesh::attribute(int attributeIndex, int dimensions, const std::vector<float> &content) {
    this->vao.vertex_buffer(attributeIndex, dimensions, content);
}


void ic::Mesh::add_attribute(int attributeIndex, int dimensions, const std::vector<float> &content) {
    this->vao.add_vertex_buffer(attributeIndex, dimensions, content);
}
void ic::Mesh::add_attribute(int attributeIndex, int dimensions, const std::vector<int> &content) {
    this->vao.add_vertex_buffer(attributeIndex, dimensions, content);
}


void ic::Mesh::add_attribute(int attributeIndex, int dimensions, const std::vector<ic::Color> &content) {
    std::vector<float> colorValues;
    for (int i = 0; i < content.size(); i++) {
        ic::Color color = content[i];

        colorValues.push_back(color.r / 255.0f);
        colorValues.push_back(color.g / 255.0f);
        colorValues.push_back(color.b / 255.0f);
    }
    add_attribute(attributeIndex, dimensions, colorValues);
}




void ic::Mesh::set_index_buffer(const std::vector<unsigned int> &content) {
    this->vao.set_index_buffer(content);
}

void ic::Mesh::unuse_attribute_definitions() {
    this->vao.unuse_attribute_definitions();
}

void ic::Mesh::draw(ic::Shader &shader, ic::GLPrimitives primitive) {
    auto drawable = this->vao.get_drawable();

    //this->upload_material(shader, this->material);
    shader.set_uniform_mat4("model", this->model);
    drawable.use_and_draw(primitive);
}

void ic::Mesh::dispose() {
    this->vao.dispose();
}

void ic::Mesh::upload_material(ic::Shader &shader, const ic::MeshMaterial2D &mat) {
    shader.set_uniform_float("material.colorBlending", mat.colorBlending);
    shader.set_uniform_vec3f("material.baseColor", mat.baseColor);
}

void ic::Mesh::using_indices(bool to) {
    this->vao.using_indices(to);
}

void ic::Mesh::set_index_count(GLsizei to) {
    this->vao.set_index_count(to);
}