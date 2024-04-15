#include <Physics/forces/Gravity.h>
#include <Physics/objects/RigidObject.h>

using namespace ic::Physics;

Gravity::Gravity() {
}

Gravity::Gravity(float x, float y, float z) {
    this->force.x() = x;
    this->force.y() = y;
    this->force.z() = z;
}

Gravity::Gravity(ic::Vec3f force) {
    this->force = force;
}


void ic::Physics::Gravity::apply(ic::Physics::Object *object) {
    ic::Physics::RigidObject *body = dynamic_cast<ic::Physics::RigidObject*>(object);

    if (body != nullptr) {
        if (body->dynamic) body->force = body->force + (this->force + body->gravity) * body->mass;
    }
}