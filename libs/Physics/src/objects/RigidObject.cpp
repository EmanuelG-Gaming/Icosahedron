#include <Physics/objects/RigidObject.h>

using namespace ic::Physics;

RigidObject::RigidObject() : Object() {
    this->mass = this->inverseMass = 1.0f;

    this->restitution = 1.0f;
    this->staticFriction = 0.0f;
    this->dynamicFriction = 0.0f;
}


ic::Physics::RigidObject *ic::Physics::RigidObject::apply_velocity(float x, float y, float z) {
    this->velocity.x() += x;
    this->velocity.y() += y;
    this->velocity.z() += z;

    return this;
}
ic::Physics::RigidObject *ic::Physics::RigidObject::apply_velocity(ic::Vec3f &vel) {
    return this->apply_velocity(vel.x(), vel.y(), vel.z());
}

ic::Physics::RigidObject *ic::Physics::RigidObject::apply_velocity(ic::Vec2 &vel) {
    return this->apply_velocity(vel.x(), vel.y(), 0.0f);
}



ic::Physics::RigidObject *ic::Physics::RigidObject::set_mass(float to) {
    this->mass = to;
    this->inverseMass = 1 / to;

    return this;
}