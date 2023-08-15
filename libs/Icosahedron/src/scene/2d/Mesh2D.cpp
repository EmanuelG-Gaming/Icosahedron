#include <Icosahedron/scene/2d/Mesh2D.h>

#include <stdexcept>

using namespace ic;

Mesh2D::Mesh2D() {
    this->vao = nullptr;
}

Mesh2D::Mesh2D(std::vector<float> vertexPositions) {
    this->vao = new ic::VertexArray();

    this->vao->add_vertex_buffer(2, vertexPositions);
    
    this->vao->unuse_attribute_definitions();
}

void ic::Mesh2D::add_attribute(const std::string &location, int dimensions, const std::vector<float> &content) {
    if (this->vao == nullptr) {
        throw std::runtime_error("Couldn't add vertex attribute. The VAO was not initialized.");
    }
    ic::GLAttribute attribute = { content, dimensions };
    this->vao->add_vertex_buffer(dimensions, content);

    vertexAttributes[location] = attribute;
}

void ic::Mesh2D::add_index_buffer(const std::vector<unsigned int> &content) {
    if (this->vao == nullptr) {
        throw std::runtime_error("Couldn't add index buffer. VAO was not initialized.");
    }
    this->vao->add_index_buffer(content);
}

ic::GLAttribute &ic::Mesh2D::get_attribute(const std::string &location) {
    return vertexAttributes[location];
}

void ic::Mesh2D::unuse_attribute_definitions() {
    if (this->vao == nullptr) return;
    this->vao->unuse_attribute_definitions();
}

void ic::Mesh2D::draw() {
    if (this->vao == nullptr) return;

    auto drawable = this->vao->get_drawable();
    drawable.use_and_draw();
}