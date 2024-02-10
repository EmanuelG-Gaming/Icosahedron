#include <Physics/3d/forces/Gravity3D.h>
#include <Physics/3d/objects/RigidObject3D.h>

using namespace ic::Physics;

Gravity3D::Gravity3D() {
}

Gravity3D::Gravity3D(float x, float y, float z) {
    this->force.x() = x;
    this->force.y() = y;
    this->force.z() = z;
}

Gravity3D::Gravity3D(ic::Vec3f force) {
    this->force = force;
}


void ic::Physics::Gravity3D::apply(ic::Physics::Object3D *object) {
    ic::Physics::RigidObject3D *body = dynamic_cast<ic::Physics::RigidObject3D*>(object);

    if (body != nullptr) {
        if (body->dynamic) body->force = body->force + (this->force + body->gravity) * body->mass;
    }
}