#include <Physics/2d/Solver2D.h>
#include <Physics/2d/objects/RigidObject2D.h>

void ic::Physics::PositionSolver2D::solve(const std::vector<ic::Physics::Manifold2D> &manifolds) {
    for (auto &manifold : manifolds) {
        if (manifold.points.depth < 0.001f) continue;

        ic::Physics::Object2D *objA = manifold.object1;
        ic::Physics::Object2D *objB = manifold.object2;

        int dynamic1 = (int) objA->dynamic;
        int dynamic2 = (int) objB->dynamic;
        
        ic::Vec2f normal = manifold.points.normal;
        ic::Vec2f displacement = normal * (1 / (float) std::max(1, dynamic1 + dynamic2));
        displacement = displacement * manifold.points.depth * 0.5f;

        objA->transform->position = objA->transform->position - displacement * dynamic1;
        objB->transform->position = objB->transform->position + displacement * dynamic2;
    }
}

void ic::Physics::ImpulseSolver2D::solve(const std::vector<ic::Physics::Manifold2D> &manifolds) {
    for (auto &manifold : manifolds) {
        if (manifold.points.depth < 0.001f) continue;

        ic::Physics::RigidObject2D *object1 = dynamic_cast<ic::Physics::RigidObject2D*>(manifold.object1);
        ic::Physics::RigidObject2D *object2 = dynamic_cast<ic::Physics::RigidObject2D*>(manifold.object2);
        if (object1 == nullptr || object2 == nullptr) continue;
        
        ic::Vec2f normal = manifold.points.normal;
        ic::Vec2f gradientVelocity = object1->velocity - object2->velocity;

        float j = 2.0f * normal.dot(gradientVelocity);
        ic::Vec2f reflectionDisplacement = normal * j;

        if (object1->dynamic && object2->dynamic) {
            // If both objects are dynamic, simply use the elastic collision formula
            float c = object1->mass + object2->mass;
            
            if (object1->dynamic) object1->velocity = object1->velocity - reflectionDisplacement / c * object2->mass;
            if (object2->dynamic) object2->velocity = object2->velocity + reflectionDisplacement / c * object1->mass;
        } else {
            // If one of the objects is completely immovable, the other object will ricochet off
            if (!object2->dynamic) object1->velocity = object1->velocity - reflectionDisplacement;
            if (!object1->dynamic) object2->velocity = object2->velocity + reflectionDisplacement;
        }
    }
}