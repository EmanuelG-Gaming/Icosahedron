#include <Physics/objects/Object.h>

using namespace ic::Physics;

Object::Object() {
    this->transform = new ic::Physics::Transform();
    this->collider = 0;
    this->onCollision = nullptr;
    
    this->dynamic = true;
    this->isTrigger = false;
    this->collides = false;
}

void ic::Physics::Object::collision(const std::function<void(ic::Physics::Manifold, float)> &to) {
    this->onCollision = to;
}

void ic::Physics::Object::set_position(float x, float y, float z) {
    this->transform->position.x() = x;
    this->transform->position.y() = y;
    this->transform->position.z() = z;
}

void ic::Physics::Object::set_position(ic::Vec3f &to) {
    this->set_position(to.x(), to.y(), to.z());
}