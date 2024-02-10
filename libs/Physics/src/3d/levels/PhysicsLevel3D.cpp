#include <Physics/3d/levels/PhysicsLevel3D.h>

using namespace ic::Physics;

PhysicsLevel3D::PhysicsLevel3D() {
    this->load();
}


void ic::Physics::PhysicsLevel3D::load() {
    simulationSteps = 1;

    timeAccumulator = 0.0f;
    fixedTimeLength = 1 / 40.0f;
    shouldResetForces = true;
    
    objects.clear();
    solvers.clear();
    
    gravity = new ic::Physics::Gravity3D();

    add_solver(new ic::Physics::PositionSolver3D());
    add_solver(new ic::Physics::ImpulseSolver3D());

    add_force(gravity);
}

void ic::Physics::PhysicsLevel3D::update(float timeTook) {
    timeAccumulator += timeTook;

    if (timeAccumulator >= fixedTimeLength) {
        float stepSize = fixedTimeLength / (float)simulationSteps;
        for (int i = 0; i < simulationSteps; i++) {
            update_with_sub_steps(stepSize);
        }

        timeAccumulator -= fixedTimeLength;
    }
}

void ic::Physics::PhysicsLevel3D::update_with_sub_steps(float timeTook) {
    // Reseting the forces + applying
    if (this->shouldResetForces) {
        for (auto &object : objects) {
            ic::Physics::RigidObject3D *body = dynamic_cast<ic::Physics::RigidObject3D*>(object);
            
            object->collides = false;

            if (body != nullptr) {
                body->force = { 0.0f, 0.0f };
            }

            for (auto &force : forces) {
                force->apply(object);
            }
        }
    }
    this->shouldResetForces = true;

    // Motion integration
    for (auto &object : objects) {
        ic::Physics::RigidObject3D *body = dynamic_cast<ic::Physics::RigidObject3D*>(object);

        if (body != nullptr) {
            // Calculate acceleration and angular acceleration based on the formula F = ma, with a = F/m
            ic::Vec3f acceleration = body->force / body->mass;
            
            // Currently uses Euler's method
            // Linear part
            body->velocity = body->velocity + acceleration * timeTook;
            body->transform->position = body->transform->position + body->velocity * timeTook;
        }
    }

    resolve_collisions(timeTook);
}


void ic::Physics::PhysicsLevel3D::add_object(ic::Physics::Object3D *object) {
    objects.push_back(object);
}
void ic::Physics::PhysicsLevel3D::remove_object(ic::Physics::Object3D *object) {
    if (!object) return;

    auto iterator = std::find(objects.begin(), objects.end(), object);
    if (iterator == objects.end()) return;

    objects.erase(iterator);
}

ic::Physics::Object3D *ic::Physics::PhysicsLevel3D::get_object(int index) {
    return objects[index];
}

void ic::Physics::PhysicsLevel3D::add_force(ic::Physics::Force3D *force) {
    forces.push_back(force);
}

void ic::Physics::PhysicsLevel3D::add_solver(ic::Physics::Solver3D *solver) {
    solvers.push_back(solver);
}
        
void ic::Physics::PhysicsLevel3D::send_collision_callbacks(const std::vector<ic::Physics::Manifold3D> &collisions, float timeTook) {
    for (auto &manifold : collisions) {
        auto &callback1 = manifold.object1->onCollision;
        auto &callback2 = manifold.object2->onCollision;

        manifold.object1->collides = true;
        manifold.object2->collides = true;

        if (callback1) callback1(manifold, timeTook);
        if (callback2) callback2(manifold, timeTook);
    }
}
        
        
void ic::Physics::PhysicsLevel3D::resolve_collisions(float timeTook) {
    manifolds.clear();
    triggers.clear();
    
    // Collision detection
    for (auto &object1 : objects) {
        for (auto &object2 : objects) {
            if (object1 == object2) break;
            if (!object1->dynamic && !object2->dynamic) continue;
            if (!object1->collider || !object2->collider) continue;
            
            ic::Physics::ManifoldPoints3D points = object1->collider->test(object1->transform, object2->collider, object2->transform);
            if (!points.collided) continue;
            
            ic::Physics::Manifold3D manifold = ic::Physics::Manifold3D(object1, object2, points);
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

std::vector<ic::Physics::Object3D*> &ic::Physics::PhysicsLevel3D::get_objects() { 
    return objects;
}

void ic::Physics::PhysicsLevel3D::set_fixed_time_length(int framesPerSecond) {
    fixedTimeLength = 1 / (float) framesPerSecond;
}

void ic::Physics::PhysicsLevel3D::set_gravity(float x, float y, float z) {
    gravity->force.x() = x;
    gravity->force.y() = y;
    gravity->force.z() = z;
}

void ic::Physics::PhysicsLevel3D::set_gravity(ic::Vec3f &force) {
    set_gravity(force.x(), force.y(), force.z());
}


void ic::Physics::PhysicsLevel3D::reset_forces() {
    for (auto &object : objects) {
        ic::Physics::RigidObject3D *body = dynamic_cast<ic::Physics::RigidObject3D*>(object);
    
        if (body != nullptr) {
            body->force = { 0.0f, 0.0f };
        }
        
        for (auto &force : forces) {
            force->apply(object);
        }
    }

    this->shouldResetForces = false;
}