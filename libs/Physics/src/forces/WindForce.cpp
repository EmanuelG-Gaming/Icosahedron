#include <Physics/forces/WindForce.h>
#include <Physics/objects/RigidObject.h>

using namespace ic::Physics;

WindForce::WindForce() {}

WindForce::WindForce(float density, float coefficient) {
    this->density = density;
    this->coefficient = coefficient;
}


void ic::Physics::WindForce::apply(ic::Physics::Object *object) {
    ic::Physics::RigidObject *body = dynamic_cast<ic::Physics::RigidObject*>(object);

    if (body != nullptr && body->dynamic) {
        ic::Vec3f difference = body->velocity - this->force;
        float area = 1.0f;
        float magnitude = 0.5f * this->density * difference.len2() * this->coefficient * area;

        ic::Vec3f opposite = difference.nor() * -1;
        body->force = body->force + opposite * magnitude + this->force;
    }
}