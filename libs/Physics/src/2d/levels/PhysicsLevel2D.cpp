#include <Physics/2d/levels/PhysicsLevel2D.h>

using namespace ic::Physics;

PhysicsLevel2D::PhysicsLevel2D() {
    this->load();
}


void ic::Physics::PhysicsLevel2D::load() {
    simulationSteps = 1;
    
    objects.clear();
    solvers.clear();
    
    add_solver(new ic::Physics::PositionSolver2D());
    add_solver(new ic::Physics::ImpulseSolver2D());
}

void ic::Physics::PhysicsLevel2D::update(float timeTook) {
    float stepSize = timeTook / (float)simulationSteps;
    for (int i = 0; i < simulationSteps; i++) {
        update_with_sub_steps(stepSize);
    }
}

void ic::Physics::PhysicsLevel2D::update_with_sub_steps(float timeTook) {
    resolve_collisions(timeTook);
                
    // Reseting the forces + applying
    for (auto &object : objects) {
        ic::Physics::RigidObject2D *body = dynamic_cast<ic::Physics::RigidObject2D*>(object);
        if (body == nullptr) continue;
        
        body->force = { 0.0f, 0.0f };
         
        //for (auto &force : forces) {
        //    force->apply(body, timeTook);
        //}
        //ic::Vec2f acceleration = body->velocity * -1 + gravity;
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