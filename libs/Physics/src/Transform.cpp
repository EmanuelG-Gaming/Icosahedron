#include <Physics/Transform.h>

using namespace ic::Physics;

Transform::Transform() {
    this->rotation = ic::Quaternion().from_euler(0.0f, 0.0f, 0.0f);
    this->scale = { 1.0f, 1.0f, 1.0f };
}