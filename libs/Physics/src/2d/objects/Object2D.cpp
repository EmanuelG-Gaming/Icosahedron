#include <Physics/2d/objects/Object2D.h>

using namespace ic::Physics;

Object2D::Object2D() {
    this->transform = new ic::Physics::Transform2D();
    this->collider = 0;
    this->onCollision = nullptr;
    
    this->dynamic = true;
    this->isTrigger = false;
}

void ic::Physics::Object2D::collision(const std::function<void(ic::Physics::Manifold2D, float)> &to) {
    this->onCollision = to;
}

void ic::Physics::Object2D::set_position(float x, float y) {
    this->transform->position.x() = x;
    this->transform->position.y() = y;
}

void ic::Physics::Object2D::set_position(ic::Vec2f &to) {
    this->set_position(to.x(), to.y());
}