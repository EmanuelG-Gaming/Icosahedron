#include <Icosahedron/scene/2d/Mesh2D.h>

#include <stdexcept>

using namespace ic;

Mesh2D::Mesh2D() {
    this->vao = new ic::VertexArray();
}

Mesh2D::Mesh2D(std::vector<float> vertexPositions) {
    this->vao = new ic::VertexArray();

    this->vao->add_vertex_buffer(2, vertexPositions);
    
    this->vao->unuse_attribute_definitions();
}

void ic::Mesh2D::set_material(ic::MeshMaterial2D newMaterial) {
    this->material = newMaterial;
}

void ic::Mesh2D::set_transformation(const ic::Mat4x4 &to) {
    this->model = to;
}

void ic::Mesh2D::add_attribute(const std::string &location, int dimensions, const std::vector<float> &content) {
    if (this->vao == nullptr) {
        throw std::runtime_error("Couldn't add vertex attribute. The VAO was not initialized.");
    }
    ic::GLAttribute attribute = { content, dimensions };
    this->vao->add_vertex_buffer(dimensions, content);

    vertexAttributes[location] = attribute;
}
void ic::Mesh2D::add_attribute(const std::string &location, int dimensions, const std::vector<ic::Color> &content) {
    std::vector<float> colorValues;
    for (int i = 0; i < content.size(); i++) {
        ic::Color color = content[i];

        colorValues.push_back(color.r / 255.0f);
        colorValues.push_back(color.g / 255.0f);
        colorValues.push_back(color.b / 255.0f);
    }
    add_attribute(location, dimensions, colorValues);
}
            
void ic::Mesh2D::set_index_buffer(const std::vector<unsigned int> &content) {
    if (this->vao == nullptr) {
        throw std::runtime_error("Couldn't add index buffer. VAO was not initialized.");
    }
    this->vao->set_index_buffer(content);
}

void ic::Mesh2D::jump_attribute() {
    if (this->vao == nullptr) {
        throw std::runtime_error("Couldn't jump to next attribute. VAO was not initialized.");
    }
    this->vao->jump();
}

ic::GLAttribute &ic::Mesh2D::get_attribute(const std::string &location) {
    return vertexAttributes[location];
}

void ic::Mesh2D::unuse_attribute_definitions() {
    if (this->vao == nullptr) return;
    this->vao->unuse_attribute_definitions();
}

void ic::Mesh2D::draw(ic::Shader *shader) {
    if (this->vao == nullptr) return;

    auto drawable = this->vao->get_drawable();

    this->upload_material(shader, this->material);
    shader->set_uniform_mat4("model", this->model);
    drawable.use_and_draw();
}

void ic::Mesh2D::dispose() {
    this->vao->dispose();
}

void ic::Mesh2D::upload_material(ic::Shader *shader, const ic::MeshMaterial2D &mat) {
    shader->set_uniform_float("material.colorBlending", mat.colorBlending);
    shader->set_uniform_vec3f("material.baseColor", mat.baseColor);
}