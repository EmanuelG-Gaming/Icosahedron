#include <Physics/2d/levels/ObjectLevel2D.h>

void ic::Physics::ObjectLevel2D::add_object(ic::Physics::Object2D *object) {
    object->index = lastIndex;
    
    objects.push_back(object);
    lastIndex++;
}

void ic::Physics::ObjectLevel2D::add_solver(ic::Physics::Solver2D *solver) {
    solvers.push_back(solver);
}
        
void ic::Physics::ObjectLevel2D::send_collision_callbacks(const std::vector<ic::Physics::Manifold2D> &collisions, float timeTook) {
    for (auto &manifold : collisions) {
        auto &callback1 = manifold.object1->onCollision;
        auto &callback2 = manifold.object2->onCollision;
        
        if (callback1) callback1();
        if (callback2) callback2();
    }
}
        
        
void ic::Physics::ObjectLevel2D::resolve_collisions(float timeTook) {
    manifolds.clear();
    triggers.clear();
    
    // Collision detection
    for (auto &object1 : objects) {
        for (auto &object2 : objects) {
            if (object1->index == object2->index) break;
            if (!object1->collider || !object2->collider) continue;
            if (!object1->dynamic && !object2->dynamic) continue;
            
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

std::vector<ic::Physics::Object2D*> &ic::Physics::ObjectLevel2D::get_objects() { 
    return objects;
}