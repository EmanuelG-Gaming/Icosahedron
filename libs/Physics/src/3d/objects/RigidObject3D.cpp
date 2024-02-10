#include <Physics/3d/objects/RigidObject3D.h>

using namespace ic::Physics;

RigidObject3D::RigidObject3D() : Object3D() {
    this->mass = 1.0f;

    this->restitution = 1.0f;
    this->staticFriction = 0.0f;
    this->dynamicFriction = 0.0f;
}


ic::Physics::RigidObject3D *ic::Physics::RigidObject3D::apply_velocity(float x, float y, float z) {
    this->velocity.x() += x;
    this->velocity.y() += y;
    this->velocity.z() += z;

    return this;
}
ic::Physics::RigidObject3D *ic::Physics::RigidObject3D::apply_velocity(ic::Vec3f &vel) {
    return this->apply_velocity(vel.x(), vel.y(), vel.z());
}


ic::Physics::RigidObject3D *ic::Physics::RigidObject3D::set_mass(float to) {
    this->mass = to;

    return this;
}