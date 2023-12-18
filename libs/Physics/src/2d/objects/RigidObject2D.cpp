#include <Physics/2d/objects/RigidObject2D.h>

using namespace ic::Physics;

RigidObject2D::RigidObject2D() : Object2D() {
    this->mass = 1.0f;
    this->inertia = 1.0f;
    
    this->torque = 0.0f;
    this->angularVelocity = 0.0f;

    this->restitution = 1.0f;
    this->staticFriction = 0.0f;
    this->dynamicFriction = 0.0f;

    this->crossSectionalArea = nullptr;
}


ic::Physics::RigidObject2D *ic::Physics::RigidObject2D::apply_velocity(float x, float y) {
    this->velocity.x() += x;
    this->velocity.y() += y;

    return this;
}
ic::Physics::RigidObject2D *ic::Physics::RigidObject2D::apply_velocity(ic::Vec2f &vel) {
    return this->apply_velocity(vel.x(), vel.y());
}


ic::Physics::RigidObject2D *ic::Physics::RigidObject2D::set_mass(float to) {
    this->mass = to;

    return this;
}


ic::Physics::RigidObject2D *ic::Physics::RigidObject2D::set_section_area(const std::function<float(ic::Vec2f)> &crossSectionalArea) {
    this->crossSectionalArea = crossSectionalArea;

    return this;
}