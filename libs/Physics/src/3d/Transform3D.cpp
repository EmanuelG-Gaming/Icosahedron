#include <Physics/3d/Transform3D.h>

using namespace ic::Physics;

Transform3D::Transform3D() {
    this->rotation = ic::Quaternion().from_euler(0.0f, 0.0f, 0.0f);
    this->scale = { 1.0f, 1.0f, 1.0f };
}