#include <Physics/2d/forces/WindForce2D.h>
#include <Physics/2d/objects/RigidObject2D.h>

using namespace ic::Physics;


WindForce2D::WindForce2D(float density, float coefficient) {
    this->density = density;
    this->coefficient = coefficient;
}

WindForce2D::WindForce2D(ic::Vec2f force, float density, float coefficient) {
    this->force = force;

    this->density = density;
    this->coefficient = coefficient;
}


void ic::Physics::WindForce2D::apply(ic::Physics::Object2D *object) {
    ic::Physics::RigidObject2D *body = dynamic_cast<ic::Physics::RigidObject2D*>(object);

    if (body != nullptr && body->dynamic) {
        ic::Vec2f difference = body->velocity - this->force;
        float area = body->crossSectionalArea == nullptr ? 1.0f : body->crossSectionalArea(difference);
        float magnitude = 0.5f * this->density * difference.len2() * this->coefficient * area;

        ic::Vec2f opposite = difference.nor() * -1;
        body->force = body->force + opposite * magnitude + this->force;
    }
}