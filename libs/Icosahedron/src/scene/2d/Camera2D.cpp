#include <Icosahedron/scene/2d/Camera2D.h>

using namespace ic;

Camera2D::Camera2D() {
    this->position = { 0.0f, 0.0f };
    this->scale = 1.0f;
}
Camera2D::Camera2D(ic::Vec2f position, float scale) {
    this->position = position;
    this->scale = scale;
}
Camera2D::Camera2D(float scale) {
    this->position = { 0.0f, 0.0f };
    this->scale = scale;
}

void Camera2D::use(ic::Shader *shader) {
    ic::Mat4x4 scaling;
    ic::Vec3f pos = { position.x(), position.y(), 0.0f };

    scaling.set_scaling(this->scale);
    projection.set_translation((pos * -1));

    projection = projection * scaling;
    
    shader->set_uniform_mat4("projection", projection);
    shader->set_uniform_bool("useCamera", true);
}
void Camera2D::unuse(ic::Shader *shader) {
    shader->set_uniform_bool("useCamera", false);
}

ic::Vec2f Camera2D::project(ic::Vec2f &worldPosition) {
    ic::Vec2f result;

    result.x() = (worldPosition.x() - position.x()) * this->scale;
    result.y() = (worldPosition.y() - position.y()) * this->scale;

    return result;
}
ic::Vec2f Camera2D::unproject(ic::Vec2f &screenPosition) {
    ic::Vec2f result;

    result.x() = screenPosition.x() / this->scale + position.x();
    result.y() = screenPosition.y() / this->scale + position.y();

    return result;
}