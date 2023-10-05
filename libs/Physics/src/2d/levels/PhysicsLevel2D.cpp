#include <Physics/2d/levels/PhysicsLevel2D.h>
#include <SDL2/SDL.h>

using namespace ic::Physics;

PhysicsLevel2D::PhysicsLevel2D() {
    this->load();
}


void ic::Physics::PhysicsLevel2D::load() {
    simulationSteps = 1;

    timeAccumulator = 0.0f;
    fixedTimeLength = 1 / 40.0f;
    
    objects.clear();
    solvers.clear();
    
    add_solver(new ic::Physics::PositionSolver2D());
    add_solver(new ic::Physics::ImpulseSolver2D());
}

void ic::Physics::PhysicsLevel2D::update(float timeTook) {
    timeAccumulator += timeTook;

    if (timeAccumulator >= fixedTimeLength) {
        float stepSize = fixedTimeLength / (float)simulationSteps;
        for (int i = 0; i < simulationSteps; i++) {
            update_with_sub_steps(stepSize);
        }

        timeAccumulator -= fixedTimeLength;
        std::cout << "Physics update.\n";
    }
}

void ic::Physics::PhysicsLevel2D::update_with_sub_steps(float timeTook) {
    resolve_collisions(timeTook);
                
    // Reseting the forces + applying
    for (auto &object : objects) {
        ic::Physics::RigidObject2D *body = dynamic_cast<ic::Physics::RigidObject2D*>(object);
        if (body == nullptr) continue;
        
        body->force = { 0.0f, 0.0f };
        body->force = body->force + gravity * body->mass;
    }

    // Motion integration
    for (auto &object : objects) {
        if (!object->dynamic) continue;
        
        ic::Physics::RigidObject2D *body = dynamic_cast<ic::Physics::RigidObject2D*>(object);
        if (body == nullptr) continue;
        
        // Calculate acceleration based on the formula F = ma, with a = F/m
        ic::Vec2f acceleration = body->force / body->mass;

        // Currently uses Euler's method
        body->velocity = body->velocity + acceleration * timeTook;
        body->transform->position = body->transform->position + body->velocity * timeTook;
    }
}


void ic::Physics::PhysicsLevel2D::set_fixed_time_length(int framesPerSecond) {
    fixedTimeLength = 1 / (float) framesPerSecond;
}

void ic::Physics::PhysicsLevel2D::set_gravity(float x, float y) {
    gravity.x() = x;
    gravity.y() = y;
}

void ic::Physics::PhysicsLevel2D::set_gravity(ic::Vec2f &acceleration) {
    set_gravity(acceleration.x(), acceleration.y());
}
