#include <Physics/3d/forces/WindForce3D.h>
#include <Physics/3d/objects/RigidObject3D.h>

using namespace ic::Physics;

WindForce3D::WindForce3D() {}

WindForce3D::WindForce3D(float density, float coefficient) {
    this->density = density;
    this->coefficient = coefficient;
}


void ic::Physics::WindForce3D::apply(ic::Physics::Object3D *object) {
    ic::Physics::RigidObject3D *body = dynamic_cast<ic::Physics::RigidObject3D*>(object);

    if (body != nullptr && body->dynamic) {
        ic::Vec3f difference = body->velocity - this->force;
        float area = 1.0f;
        float magnitude = 0.5f * this->density * difference.len2() * this->coefficient * area;

        ic::Vec3f opposite = difference.nor() * -1;
        body->force = body->force + opposite * magnitude + this->force;
    }
}