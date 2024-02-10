#include <Physics/3d/objects/Object3D.h>

using namespace ic::Physics;

Object3D::Object3D() {
    this->transform = new ic::Physics::Transform3D();
    this->collider = 0;
    this->onCollision = nullptr;
    
    this->dynamic = true;
    this->isTrigger = false;
    this->collides = false;
}

void ic::Physics::Object3D::collision(const std::function<void(ic::Physics::Manifold3D, float)> &to) {
    this->onCollision = to;
}

void ic::Physics::Object3D::set_position(float x, float y, float z) {
    this->transform->position.x() = x;
    this->transform->position.y() = y;
    this->transform->position.z() = z;
}

void ic::Physics::Object3D::set_position(ic::Vec3f &to) {
    this->set_position(to.x(), to.y(), to.z());
}