#include <Icosahedron/scene/3d/controllers/FreeRoamCameraController3D.h>

using namespace ic;

FreeRoamCameraController3D::FreeRoamCameraController3D(ic::Camera3D *camera, ic::InputHandler *handler) {
    this->camera = camera;
    this->angularVelocity = { 0, 0 };
    this->rotation = { 0, 0 };

    this->keyboard = new ic::KeyboardController();
    this->mouse = new ic::MouseController();
    
    handler->add_input(this->keyboard, "freeRoam3DKeyboard");
    handler->add_input(this->mouse, "freeRoam3DMouse");
}

void ic::FreeRoamCameraController3D::act(float dt) {
    float speed = 3.0f, sensitivity = 0.5f;
    
    ic::Vec2i motion = this->mouse->get_relative_motion();
    if (motion.len2() < 4 * 4) motion = { 0, 0 };
    
    ic::Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    if (this->keyboard->is_key_pressed(KEY_W) || this->keyboard->is_key_pressed(KEY_UP)) {
        // Forward
        velocity.x() += ic::Mathf::get().cosf(rotation.x());
        velocity.z() += ic::Mathf::get().sinf(rotation.x());
    }
    if (this->keyboard->is_key_pressed(KEY_S) || this->keyboard->is_key_pressed(KEY_DOWN)) {
        // Backward
        velocity.x() += -ic::Mathf::get().cosf(rotation.x());
        velocity.z() += -ic::Mathf::get().sinf(rotation.x());
    }
    if (this->keyboard->is_key_pressed(KEY_A) || this->keyboard->is_key_pressed(KEY_LEFT)) {
        // Strafe left
        velocity.x() += ic::Mathf::get().cosf(rotation.x() + ic::Mathf::get().pi / 2.0f);
        velocity.z() += ic::Mathf::get().sinf(rotation.x() + ic::Mathf::get().pi / 2.0f);
    }
    if (this->keyboard->is_key_pressed(KEY_D) || this->keyboard->is_key_pressed(KEY_RIGHT)) {
        // Strafe right
        velocity.x() += -ic::Mathf::get().cosf(rotation.x() + ic::Mathf::get().pi / 2.0f);
        velocity.z() += -ic::Mathf::get().sinf(rotation.x() + ic::Mathf::get().pi / 2.0f);
    }

    camera->position.x() += velocity.x() * speed * dt;
    camera->position.y() += velocity.y() * speed * dt;
    camera->position.z() += velocity.z() * speed * dt;


    rotation.x() -= motion.x() * sensitivity * dt;
    rotation.y() -= motion.y() * sensitivity * dt;

    if (rotation.x() > 2 * M_PI) rotation.x() = 0;
    if (rotation.x() < 0) rotation.x() = 2 * M_PI;
       
    if (rotation.y() > (89.0f / 180.0f * M_PI)) rotation.y() = (89.0f / 180.0f * M_PI);
    if (rotation.y() < -(89.0f / 180.0f * M_PI)) rotation.y() = -(89.0f / 180.0f * M_PI);
      
    camera->lookingAt.x() = camera->position.x() + cos(rotation.x()) * cos(rotation.y());
    camera->lookingAt.y() = camera->position.y() + sin(rotation.y());
    camera->lookingAt.z() = camera->position.z() + sin(rotation.x()) * cos(rotation.y());      
}