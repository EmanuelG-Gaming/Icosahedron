#include <Icosahedron/scene/3d/controllers/FreeRoamCameraController3D.h>

using namespace ic;


FreeRoamCameraController3D::FreeRoamCameraController3D() {
    this->rotation = { 0, 0 };

    this->flying = false;
    this->speed = 3.0f;
    this->lookSensitivity = 1.0f;
}

FreeRoamCameraController3D::FreeRoamCameraController3D(ic::Camera3D *camera) {
    this->camera = camera;
    this->rotation = { 0, 0 };

    this->flying = false;
    this->speed = 3.0f;
    this->lookSensitivity = 1.0f;
    
    
    this->keyboard = new ic::KeyboardController();
    this->mouse = new ic::MouseController();
    
    ic::InputHandler::get().add_input(this->keyboard, "freeRoam3DKeyboard");
    ic::InputHandler::get().add_input(this->mouse, "freeRoam3DMouse");
}


void ic::FreeRoamCameraController3D::act(float dt) {
    ic::Vec3f offset = {
        cos(rotation.x()) * cos(rotation.y()),
        sin(rotation.y()),
        sin(rotation.x()) * cos(rotation.y())
    };

    ic::Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    if (this->flying) {
        ic::Vec3f right = offset.crs(this->camera->up);
        ic::Vec3f up = offset.crs(right);
        ic::Vec3f strafe = up.crs(offset).nor();

        if (this->keyboard->is_key_pressed(KEY_W) || this->keyboard->is_key_pressed(KEY_UP)) {
            // Forward
            velocity = velocity + offset;
        }
        if (this->keyboard->is_key_pressed(KEY_S) || this->keyboard->is_key_pressed(KEY_DOWN)) {
            // Backward
            velocity = velocity - offset;
        }
        if (this->keyboard->is_key_pressed(KEY_A) || this->keyboard->is_key_pressed(KEY_LEFT)) {
            // Strafe left
            velocity = velocity + strafe;
        }
        if (this->keyboard->is_key_pressed(KEY_D) || this->keyboard->is_key_pressed(KEY_RIGHT)) {
            // Strafe right
            velocity = velocity - strafe;
        }
    } else {
        if (this->keyboard->is_key_pressed(KEY_W) || this->keyboard->is_key_pressed(KEY_UP)) {
            // Forward
            velocity.x() += cos(rotation.x());
            velocity.z() += sin(rotation.x());
        }
        if (this->keyboard->is_key_pressed(KEY_S) || this->keyboard->is_key_pressed(KEY_DOWN)) {
            // Backward
            velocity.x() -= cos(rotation.x());
            velocity.z() -= sin(rotation.x());
        }
        if (this->keyboard->is_key_pressed(KEY_A) || this->keyboard->is_key_pressed(KEY_LEFT)) {
            // Strafe left
            velocity.x() += ic::Mathf::get().cosf(rotation.x() + ic::Mathf::get().pi / 2.0f);
            velocity.z() += ic::Mathf::get().sinf(rotation.x() + ic::Mathf::get().pi / 2.0f);
        }
        if (this->keyboard->is_key_pressed(KEY_D) || this->keyboard->is_key_pressed(KEY_RIGHT)) {
            // Strafe right
            velocity.x() -= ic::Mathf::get().cosf(rotation.x() + ic::Mathf::get().pi / 2.0f);
            velocity.z() -= ic::Mathf::get().sinf(rotation.x() + ic::Mathf::get().pi / 2.0f);
        }
    }

    ic::Vec2i motion = this->mouse->get_relative_motion();
    if (motion.len2() < 16) motion = { 0, 0 };
    
    rotation.x() -= motion.x() * this->lookSensitivity * dt;
    rotation.y() -= motion.y() * this->lookSensitivity * dt;

    if (rotation.x() > 2 * M_PI) rotation.x() = 0.0f;
      
    if (rotation.y() > (89.0f / 180.0f * M_PI)) rotation.y() = (89.0f / 180.0f * M_PI);
    if (rotation.y() < -(89.0f / 180.0f * M_PI)) rotation.y() = -(89.0f / 180.0f * M_PI);
    
    
    camera->position = velocity * speed * dt + camera->position;
    camera->lookingAt = camera->position + offset;
}