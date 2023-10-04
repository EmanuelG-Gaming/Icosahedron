#include <Physics/2d/objects/RigidObject2D.h>

using namespace ic::Physics;

RigidObject2D::RigidObject2D() : Object2D() {
    this->mass = 1.0f;
}


void ic::Physics::RigidObject2D::apply_velocity(float x, float y) {
    this->velocity.x() += x;
    this->velocity.y() += y;
}
void ic::Physics::RigidObject2D::apply_velocity(ic::Vec2f &vel) {
    this->apply_velocity(vel.x(), vel.y());
}


void ic::Physics::RigidObject2D::set_mass(float to) {
    this->mass = to;
}