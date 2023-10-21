#include <Physics/2d/forces/Gravity2D.h>
#include <Physics/2d/objects/RigidObject2D.h>

using namespace ic::Physics;

Gravity2D::Gravity2D() {
}

Gravity2D::Gravity2D(float x, float y) {
    this->force.x() = x;
    this->force.y() = y;
}

Gravity2D::Gravity2D(ic::Vec2f force) {
    this->force = force;
}


void ic::Physics::Gravity2D::apply(ic::Physics::Object2D *object) {
    ic::Physics::RigidObject2D *body = dynamic_cast<ic::Physics::RigidObject2D*>(object);

    if (body != nullptr) {
        if (body->dynamic) body->force = body->force + (this->force + body->gravity) * body->mass;
    }
}