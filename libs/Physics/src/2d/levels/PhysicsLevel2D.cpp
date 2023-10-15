#include <Physics/2d/levels/PhysicsLevel2D.h>

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
    }
}

void ic::Physics::PhysicsLevel2D::update_with_sub_steps(float timeTook) {
    resolve_collisions(timeTook);
                
    // Reseting the forces + applying
    for (auto &object : objects) {
        ic::Physics::RigidObject2D *body = dynamic_cast<ic::Physics::RigidObject2D*>(object);
        ic::Physics::SpringMassSystem2D *system = dynamic_cast<ic::Physics::SpringMassSystem2D*>(object);

        if (body != nullptr) {
            body->force = { 0.0f, 0.0f };
            body->torque = 0.0f;

            if (body->dynamic) body->force = body->force + (gravity + body->gravity) * body->mass;

        } else if (system != nullptr) {
            for (auto &force : system->get_vertex_forces()) {
                force = { 0.0f, 0.0f };
                if (system->dynamic) force = force + gravity * system->pointMass;
            }

        }
    }

    // Motion integration
    for (auto &object : objects) {
        ic::Physics::RigidObject2D *body = dynamic_cast<ic::Physics::RigidObject2D*>(object);
        ic::Physics::SpringMassSystem2D *system = dynamic_cast<ic::Physics::SpringMassSystem2D*>(object);

        if (body != nullptr) {
            // Calculate acceleration and angular acceleration based on the formula F = ma, with a = F/m
            ic::Vec2f acceleration = body->force / body->mass;
            float angularAcceleration = body->torque / body->inertia;

            // Currently uses Euler's method
            // Linear part
            body->velocity = body->velocity + acceleration * timeTook;
            body->transform->position = body->transform->position + body->velocity * timeTook;

            // Angular part
            body->angularVelocity = body->angularVelocity + angularAcceleration * timeTook;
            body->transform->angle = body->transform->angle + body->angularVelocity * timeTook;
            
        } else if (system != nullptr) {
            for (int i = 0; i < system->get_vertex_positions().size(); i++) {
                ic::Vec2f &force = system->get_vertex_forces()[i];
                ic::Vec2f &velocity = system->get_vertex_velocities()[i];
                ic::Vec2f &position = system->get_vertex_positions()[i];
                
                ic::Vec2f acceleration = force / system->pointMass;
        
                velocity = velocity + acceleration * timeTook;
                position = position + velocity * timeTook;
            }
        }
    }
}


void ic::Physics::PhysicsLevel2D::add_object(ic::Physics::Object2D *object) {
    objects.push_back(object);
}
void ic::Physics::PhysicsLevel2D::remove_object(ic::Physics::Object2D *object) {
    if (!object) return;

    auto iterator = std::find(objects.begin(), objects.end(), object);
    if (iterator == objects.end()) return;

    objects.erase(iterator);
}

ic::Physics::Object2D *ic::Physics::PhysicsLevel2D::get_object(int index) {
    return objects[index];
}


void ic::Physics::PhysicsLevel2D::add_solver(ic::Physics::Solver2D *solver) {
    solvers.push_back(solver);
}
        
void ic::Physics::PhysicsLevel2D::send_collision_callbacks(const std::vector<ic::Physics::Manifold2D> &collisions, float timeTook) {
    for (auto &manifold : collisions) {
        auto &callback1 = manifold.object1->onCollision;
        auto &callback2 = manifold.object2->onCollision;
        
        if (callback1) callback1(manifold, timeTook);
        if (callback2) callback2(manifold, timeTook);
    }
}
        
        
void ic::Physics::PhysicsLevel2D::resolve_collisions(float timeTook) {
    manifolds.clear();
    triggers.clear();
    
    // Collision detection
    for (auto &object1 : objects) {
        for (auto &object2 : objects) {
            if (object1 == object2) break;
            if (!object1->dynamic && !object2->dynamic) continue;
            if (!object1->collider || !object2->collider) continue;
            
            ic::Physics::ManifoldPoints2D points = object1->collider->test(object1->transform, object2->collider, object2->transform);
            if (!points.collided) continue;
            
            ic::Physics::Manifold2D manifold = ic::Physics::Manifold2D(object1, object2, points);
            bool trigger = object1->isTrigger || object2->isTrigger;
            
            if (trigger) triggers.emplace_back(manifold);
            else manifolds.emplace_back(manifold);
        }
    }
    
    
    // Don't resolve collisions with triggers
    for (auto &solver : solvers) {
         solver->solve(manifolds);
    }
    send_collision_callbacks(manifolds, timeTook);
    send_collision_callbacks(triggers, timeTook);
}

std::vector<ic::Physics::Object2D*> &ic::Physics::PhysicsLevel2D::get_objects() { 
    return objects;
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
