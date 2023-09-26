#include <Icosahedron/scene/3d/Mesh3D.h>

#include <stdexcept>

using namespace ic;

Mesh3D::Mesh3D() {
    this->vao = new ic::VertexArray();
}

void ic::Mesh3D::set_transformation(const ic::Mat4x4 &to) {
    this->model = to;
}
void ic::Mesh3D::set_normal_transformation(const ic::Mat4x4 &to) {
    this->normalModel = to;
}

void ic::Mesh3D::add_attribute(const std::string &location, int attributeIndex, int dimensions, const std::vector<float> &content) {
    if (this->vao == nullptr) {
        throw std::runtime_error("Couldn't add vertex attribute. The VAO was not initialized.");
    }
    this->vao->add_vertex_buffer(attributeIndex, dimensions, content);
}
void ic::Mesh3D::add_attribute(const std::string &location, int attributeIndex, int dimensions, const std::vector<int> &content) {
    if (this->vao == nullptr) {
        throw std::runtime_error("Couldn't add vertex attribute. The VAO was not initialized.");
    }
    this->vao->add_vertex_buffer(attributeIndex, dimensions, content);
}
void ic::Mesh3D::add_attribute(const std::string &location, int attributeIndex, int dimensions, const std::vector<ic::Color> &content) {
    std::vector<float> colorValues;
    for (int i = 0; i < content.size(); i++) {
        ic::Color color = content[i];

        colorValues.push_back(color.r / 255.0f);
        colorValues.push_back(color.g / 255.0f);
        colorValues.push_back(color.b / 255.0f);
    }
    add_attribute(location, attributeIndex, dimensions, colorValues);
}




void ic::Mesh3D::add_attribute(const std::string &location, int attributeIndex, const std::vector<ic::Vec2f> &content) {
    std::vector<float> values;
    for (int i = 0; i < content.size(); i++) {
        ic::Vec2f vector = content[i];

        values.push_back(vector.x());
        values.push_back(vector.y());
    }
    add_attribute(location, attributeIndex, 2, values);
}
void ic::Mesh3D::add_attribute(const std::string &location, int attributeIndex, const std::vector<ic::Vec3f> &content) {
    std::vector<float> values;
    for (int i = 0; i < content.size(); i++) {
        ic::Vec3f vector = content[i];

        values.push_back(vector.x());
        values.push_back(vector.y());
        values.push_back(vector.z());
    }
    add_attribute(location, attributeIndex, 3, values);
}
    

void ic::Mesh3D::set_index_buffer(const std::vector<unsigned int> &content) {
    if (this->vao == nullptr) {
        throw std::runtime_error("Couldn't add index buffer. VAO was not initialized.");
    }
    this->vao->set_index_buffer(content);
}

void ic::Mesh3D::unuse_attribute_definitions() {
    if (this->vao == nullptr) return;
    this->vao->unuse_attribute_definitions();
}

void ic::Mesh3D::draw(ic::Shader *shader, ic::GLPrimitives primitive) {
    if (this->vao == nullptr) return;

    auto drawable = this->vao->get_drawable();

    shader->set_uniform_mat4("model", this->model);
    shader->set_uniform_mat4("normalModel", this->normalModel);
    drawable.use_and_draw(primitive);
}

void ic::Mesh3D::dispose() {
    this->vao->dispose();
}