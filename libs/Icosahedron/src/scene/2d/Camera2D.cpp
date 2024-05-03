#include <Icosahedron/scene/2d/Camera2D.h>

using namespace ic;

Camera2D::Camera2D() : width(IC_WINDOW_WIDTH), height(IC_WINDOW_HEIGHT) {
    this->scale = 1.0f;
    this->correctsAspectRatio = true;
}
Camera2D::Camera2D(ic::Vec2f position, float scale) : Camera2D() {
    this->position = position;
    this->scale = scale;
}

Camera2D::Camera2D(float scale) : Camera2D() {
    this->scale = scale;
}

void Camera2D::use(ic::Shader &shader) {
    ic::Mat4x4 translation, scaling;
    ic::Vec3f pos = { position.x(), position.y(), 0.0f };
    float aspect = this->width / (float) this->height;

    scaling.set_scaling(this->scale);
    translation.set_translation((pos * -1));
    if (this->correctsAspectRatio) this->aspectRatioCorrection.set_orthographic(-aspect, aspect, -1, 1);

    projection = this->aspectRatioCorrection * scaling * translation;

    shader.set_uniform_mat4("projection", projection);
}

void Camera2D::unuse(ic::Shader &shader) {
    shader.set_uniform_mat4("projection", ic::Mat4x4());
}

ic::Vec2f Camera2D::project(const ic::Vec2f &worldPosition) {
    ic::Vec2f result;

    result.x() = (worldPosition.x() - position.x()) * this->scale;
    result.y() = (worldPosition.y() - position.y()) * this->scale;

    return result;
}

ic::Vec2f Camera2D::unproject(const ic::Vec2f &screenPosition) {
    ic::Vec2f result;

    ic::Vec2f pos = screenPosition;
    //pos.x() /= width;
    //pos.y() /= height;
//
    //pos.x() *= 2.0f;
    //pos.y() *= 2.0f;
    //pos.x() -= 1.0f;
    //pos.y() -= 1.0f;
    //pos.y() *= -1.0f;


    result.x() = (pos.x() / this->scale) + position.x(); //* (width / (float) height) + position.x();
    result.y() = (pos.y() / this->scale) + position.y();

    return result;
}



void ic::Camera2D::resize(int width, int height) {
    this->width = width;
    this->height = height;
}