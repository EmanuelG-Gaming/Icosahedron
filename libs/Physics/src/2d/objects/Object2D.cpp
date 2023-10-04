#include <Physics/2d/objects/Object2D.h>

using namespace ic::Physics;

Object2D::Object2D() {
    this->transform = new ic::Physics::Transform2D();
    this->collider = 0;
    this->onCollision = nullptr;
    
    this->dynamic = true;
    this->isTrigger = false;
    this->index = 0;
}

void ic::Physics::Object2D::collision(const std::function<void()> &to) {
    this->onCollision = to;
}