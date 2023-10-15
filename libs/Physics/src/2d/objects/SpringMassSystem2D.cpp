#include <Physics/2d/objects/SpringMassSystem2D.h>

using namespace ic::Physics;


SpringMassSystem2D::SpringMassSystem2D(float damping, float stiffness, float pressureConstant) {
    this->damping = damping;
    this->stiffness = stiffness;
    this->pressureConstant = pressureConstant;

    this->pointMass = 1.0f;
    this->isClosed = true;
    //this->collider = new ic::Physics::SpringMassCollider2D();
}

void ic::Physics::SpringMassSystem2D::add_vertex(ic::Vec2f &position) {
    add_vertex(position.x(), position.y());
}

void ic::Physics::SpringMassSystem2D::add_vertex(float x, float y) {
    if (positions.size() != 0) {
        ic::Spring2D *spring = new ic::Spring2D();
        springs.push_back(spring);
    }

    positions.push_back({ x, y });
    velocities.push_back({ 0.0f, 0.0f });
    forces.push_back({ 0.0f, 0.0f });


    if (!isClosed) return;
    int newSize = positions.size();

    // Close the loop with a spring that attaches
    // the first and the last vertex
    if (newSize >= 3) {
        ic::Spring2D *spring = new ic::Spring2D();
        springs.push_back(spring);
    }

    // Delete the second last spring if necessary
    if (newSize > 3) {
        springs.erase(springs.end() - 1);
    }
}

void ic::Physics::SpringMassSystem2D::set_closed(bool to) {
    if (this->isClosed == to) return;
    this->isClosed = to;

    if (!this->isClosed) {
        springs.erase(springs.end());
        return;
    }

    if (springs.size() >= 2) {
        ic::Spring2D *spring = new ic::Spring2D();
        springs.push_back(spring);
    }
}

float ic::Physics::SpringMassSystem2D::area() {
    float result = 0.0f;

    for (int i = 1; i < positions.size(); i++) {
        ic::Vec2f position1 = positions[i - 1];
        ic::Vec2f position2 = positions[i];
        
        ic::Vec2f gradient = position2 - position1;
        float len = gradient.len();

        result += 0.5f * fabs(position2.x() - position1.x()) * fabs(gradient.perpendicular(-1).x()) * len;
    }

    return result;
}


std::vector<ic::Vec2f> &ic::Physics::SpringMassSystem2D::get_vertex_positions() {
    return positions;
}
std::vector<ic::Vec2f> &ic::Physics::SpringMassSystem2D::get_vertex_velocities() {
    return velocities;
}
std::vector<ic::Vec2f> &ic::Physics::SpringMassSystem2D::get_vertex_forces() {
    return forces;
}