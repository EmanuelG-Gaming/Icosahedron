#include <Physics/levels/PhysicsLevel.h>

using namespace ic::Physics;

PhysicsLevel::PhysicsLevel() {
    this->load();
}


void ic::Physics::PhysicsLevel::load() {
    simulationSteps = 1;

    timeAccumulator = 0.0f;
    fixedTimeLength = 1 / 40.0f;
    
    objects.clear();
    solvers.clear();
    
    gravity = new ic::Physics::Gravity();

    add_solver(new ic::Physics::PositionSolver());
    add_solver(new ic::Physics::ImpulseSolver());

    add_force(gravity);
}

void ic::Physics::PhysicsLevel::update(float timeTook, const std::function<void(float)> &consumer) {
    timeAccumulator += timeTook;

    if (timeAccumulator >= fixedTimeLength) {
        float stepSize = fixedTimeLength / (float)simulationSteps;
        for (int i = 0; i < simulationSteps; i++) {
            if (consumer) {
                consumer(stepSize);
            } else {
                update_with_sub_steps(stepSize);
            }
        }

        timeAccumulator -= fixedTimeLength;
    }
}

void ic::Physics::PhysicsLevel::update_with_sub_steps(float timeTook) {
    // Reseting the forces + applying
    this->reset_forces();

    // Motion integration
    this->integrate(timeTook);

    // Resolve collisions
    resolve_collisions(timeTook);
}

void ic::Physics::PhysicsLevel::integrate(float timeTook) {
    for (auto &object : objects) {
        ic::Physics::RigidObject *body = dynamic_cast<ic::Physics::RigidObject*>(object);

        if (body != nullptr) {
            // Calculate acceleration and angular acceleration based on the formula F = ma, with a = F/m
            ic::Vec3f acceleration = body->force * body->inverseMass;
            
            // Currently uses Euler's method
            // Linear part
            body->velocity = body->velocity + acceleration * timeTook;
            body->transform->position = body->transform->position + body->velocity * timeTook;
        }
    }
}


void ic::Physics::PhysicsLevel::add_object(ic::Physics::Object *object) {
    objects.push_back(object);
}
void ic::Physics::PhysicsLevel::remove_object(ic::Physics::Object *object) {
    if (!object) return;

    auto iterator = std::find(objects.begin(), objects.end(), object);
    if (iterator == objects.end()) return;

    objects.erase(iterator);
}

ic::Physics::Object *ic::Physics::PhysicsLevel::get_object(int index) {
    return objects[index];
}

void ic::Physics::PhysicsLevel::add_force(ic::Physics::Force *force) {
    forces.push_back(force);
}

void ic::Physics::PhysicsLevel::add_solver(ic::Physics::Solver *solver) {
    solvers.push_back(solver);
}
        
void ic::Physics::PhysicsLevel::send_collision_callbacks(const std::vector<ic::Physics::Manifold> &collisions, float timeTook) {
    for (auto &manifold : collisions) {
        auto &callback1 = manifold.object1->onCollision;
        auto &callback2 = manifold.object2->onCollision;

        manifold.object1->collides = true;
        manifold.object2->collides = true;

        if (callback1) callback1(manifold, timeTook);
        if (callback2) callback2(manifold, timeTook);
    }
}
        
        
void ic::Physics::PhysicsLevel::resolve_collisions(float timeTook) {
    manifolds.clear();
    triggers.clear();
    
    // Collision detection
    for (auto &object1 : objects) {
        for (auto &object2 : objects) {
            if (object1 == object2) break;
            if (!object1->dynamic && !object2->dynamic) continue;
            if (!object1->collider || !object2->collider) continue;
            
            ic::Physics::ManifoldPoints points = object1->collider->test(object1->transform, object2->collider, object2->transform);
            if (!points.collided) continue;
            
            ic::Physics::Manifold manifold = ic::Physics::Manifold(object1, object2, points);
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

std::vector<ic::Physics::Object*> &ic::Physics::PhysicsLevel::get_objects() { 
    return objects;
}

void ic::Physics::PhysicsLevel::set_fixed_time_length(int framesPerSecond) {
    fixedTimeLength = 1 / (float) framesPerSecond;
}

void ic::Physics::PhysicsLevel::set_gravity(float x, float y, float z) {
    gravity->force.x() = x;
    gravity->force.y() = y;
    gravity->force.z() = z;
}

void ic::Physics::PhysicsLevel::set_gravity(ic::Vec3f &force) {
    set_gravity(force.x(), force.y(), force.z());
}



void ic::Physics::PhysicsLevel::reset_forces() {
    for (auto &object : objects) {
        ic::Physics::RigidObject *body = dynamic_cast<ic::Physics::RigidObject*>(object);
    
        if (body != nullptr) {
            body->force = { 0.0f, 0.0f };
        }
        
        for (auto &force : forces) {
            force->apply(object);
        }
    }
}