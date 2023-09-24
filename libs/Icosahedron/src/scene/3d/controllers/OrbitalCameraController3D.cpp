#include <Icosahedron/scene/3d/controllers/OrbitalCameraController3D.h>

using namespace ic;

OrbitalCameraController3D::OrbitalCameraController3D(ic::Camera3D *camera, ic::InputHandler *handler) {
    this->camera = camera;
    this->rotation = { 0.0f, 0.0f };
    this->center = { 0.0f, 0.0f, 0.0f };

    this->lookSensitivity = 1.0f;
    this->radius = 2.0f;
    
    this->mouse = new ic::MouseController();
    
    handler->add_input(this->mouse, "orbital3DMouse");
}

void ic::OrbitalCameraController3D::act(float dt) {
    ic::Vec2i motion = this->mouse->get_relative_motion();
    
    rotation.x() -= motion.x() * this->lookSensitivity * dt;
    rotation.y() += motion.y() * this->lookSensitivity * dt;

    if (rotation.x() > 2 * M_PI) rotation.x() = 0.0f;
    if (rotation.x() < 0) rotation.x() = 2 * M_PI;
       
    if (rotation.y() > (89.0f / 180.0f * M_PI)) rotation.y() = (89.0f / 180.0f * M_PI);
    if (rotation.y() < -(89.0f / 180.0f * M_PI)) rotation.y() = -(89.0f / 180.0f * M_PI);

    ic::Vec3f offset = {
        cos(rotation.x()) * cos(rotation.y()),
        sin(rotation.y()),
        sin(rotation.x()) * cos(rotation.y())
    };

    camera->position = offset * this->radius + this->center;
    camera->lookingAt = this->center;
}