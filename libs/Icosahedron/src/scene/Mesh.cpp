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

void ic::Mesh::set_normal_transformation(const ic::Mat4x4 &to) {
    this->normalModel = to;
}

void ic::Mesh::combine_normal_transformation(ic::Mat4x4 &with) {
    this->normalModel = this->normalModel * with;
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

void ic::Mesh::draw(ic::GLPrimitives primitive) {
    auto drawable = this->vao.get_drawable();

    drawable.use_and_draw(primitive);
}

void ic::Mesh::apply_transformations(ic::Shader &shader, const char *modelMatName, const char *normalModelMatName) {
    shader.set_uniform_mat4(modelMatName, this->model);
    shader.set_uniform_mat4(normalModelMatName, this->normalModel);
}


void ic::Mesh::dispose() {
    this->vao.dispose();
}


void ic::Mesh::using_indices(bool to) {
    this->vao.using_indices(to);
}

void ic::Mesh::set_index_count(GLsizei to) {
    this->vao.set_index_count(to);
}