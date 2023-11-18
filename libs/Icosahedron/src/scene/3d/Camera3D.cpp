#include <Icosahedron/scene/3d/Camera3D.h>

using namespace ic;

Camera3D::Camera3D() {
    this->position = { 0.0f, 0.0f, 0.0f };
    this->up = { 0.0f, 1.0f, 0.0f };
    this->lookingAt = { 1.0f, 0.0f, 0.0f };
    this->correctsAspectRatio = true;
}

Camera3D::Camera3D(float fov, float zNear, float zFar, float width, float height, bool perspective) : Camera3D() {
    this->settings.width = width;
    this->settings.height = height;

    this->settings.fov = fov;
    this->settings.zNear = zNear;
    this->settings.zFar = zFar;
    
    this->settings.perspective = perspective;
}
Camera3D::Camera3D(bool perspective) : Camera3D() {
    this->settings.perspective = perspective;
}

void ic::Camera3D::update() {
    this->viewMatrix.set_look_at(this->position, this->lookingAt, this->up);

    float width = this->settings.width;
    float height = this->settings.height;
    float aspectRatio = width / height;
    
    if (this->settings.perspective) {
        this->projectionMatrix.set_perspective(settings.fov, settings.zNear, settings.zFar, aspectRatio);
    } else {
        this->projectionMatrix.set_orthographic(-width / 2, width / 2, -height / 2, height / 2, settings.zNear, settings.zFar);
    }
    if (this->correctsAspectRatio) this->aspectRatioCorrection.set_orthographic(-aspectRatio, aspectRatio, -1, 1);

    this->combined = this->aspectRatioCorrection * this->projectionMatrix * this->viewMatrix;
}

void ic::Camera3D::upload_to_shader(ic::Shader &shader, const std::string &viewUniform, const std::string &projectionUniform) {
    shader.set_uniform_mat4(viewUniform, this->viewMatrix);
    shader.set_uniform_mat4(projectionUniform, this->projectionMatrix);
}


ic::Mat4x4 &ic::Camera3D::get_view() {
    return this->viewMatrix;
}
ic::Mat4x4 &ic::Camera3D::get_projection() {
    return this->projectionMatrix;
}

void ic::Camera3D::resize(float width, float height) {
    this->settings.width = width;
    this->settings.height = height;
}


ic::Vec3f ic::Camera3D::unproject(ic::Vec2f &screenPosition) {
    float x = (2.0f * screenPosition.x()) / this->settings.width - 1.0f;
    float y = 1.0f - (2.0f * screenPosition.y()) / this->settings.height;
    ic::Vec4f beforeClip = { x, y, -1.0f, 1.0f };

    ic::Vec4f eyeCoords = this->projectionMatrix.inverse() * beforeClip;
    eyeCoords = { eyeCoords.x(), eyeCoords.y(), -1.0f, 0.0f };

    ic::Vec4f worldCoords = this->viewMatrix.inverse() * eyeCoords;
    ic::Vec3f result = { worldCoords.x(), worldCoords.y(), worldCoords.z() };

    return result;
}