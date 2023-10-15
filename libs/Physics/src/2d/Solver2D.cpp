#include <Physics/2d/Solver2D.h>
#include <Physics/2d/objects/RigidObject2D.h>

void ic::Physics::PositionSolver2D::solve(const std::vector<ic::Physics::Manifold2D> &manifolds) {
    for (auto &manifold : manifolds) {
        ic::Physics::Object2D *objA = manifold.object1;
        ic::Physics::Object2D *objB = manifold.object2;

        int immovable1 = (int) !objA->dynamic;
        int immovable2 = (int) !objB->dynamic;
        
        ic::Vec2f normal = manifold.points.normal;
        ic::Vec2f displacement = normal * (1.0f / (float) std::max(1, immovable1 + immovable2)) * manifold.points.depth * 0.5f;

        objA->transform->position = objA->transform->position - displacement * (1 - immovable1);
        objB->transform->position = objB->transform->position + displacement * (1 - immovable2);
    }
}

void ic::Physics::ImpulseSolver2D::solve(const std::vector<ic::Physics::Manifold2D> &manifolds) {
    for (auto &manifold : manifolds) {
        ic::Physics::RigidObject2D *object1 = dynamic_cast<ic::Physics::RigidObject2D*>(manifold.object1);
        ic::Physics::RigidObject2D *object2 = dynamic_cast<ic::Physics::RigidObject2D*>(manifold.object2);
        if (object1 == nullptr || object2 == nullptr) continue;
        
        ic::Vec2f vel1 = object1->dynamic ? object1->velocity : object2->velocity * -1.0f;
        ic::Vec2f vel2 = object2->dynamic ? object2->velocity : object1->velocity * -1.0f;
        ic::Vec2f vel1f = object1->velocity;
        ic::Vec2f vel2f = object2->velocity;

        ic::Vec2f normal = manifold.points.normal;

        ic::Vec2f gradientVelocity = vel2 - vel1;
        ic::Vec2f gradientVelocityF = vel2f - vel1f;
        
        // Prevent the player's object from "sticking" on the floor when he jumps
        if (normal.dot(gradientVelocityF) > 0.0f) {
            continue;
        }

        float m1 = object1->dynamic ? object1->mass : object2->mass;
        float m2 = object2->dynamic ? object2->mass : object1->mass;
        float e = std::min(object1->restitution, object2->restitution);
        float j = -(1.0f + e) * normal.dot(gradientVelocity) / (m1 + m2);
        
        // Object reflection impulse
        {
            ic::Vec2f reflectionDisplacement = normal * j;
            if (object1->dynamic) object1->velocity = object1->velocity - reflectionDisplacement * m2;
            if (object2->dynamic) object2->velocity = object2->velocity + reflectionDisplacement * m1;
        }

        // Object friction
        {
            float staticFriction =  (object1->staticFriction + object2->staticFriction)   * 0.5f;
            float dynamicFriction = (object1->dynamicFriction + object2->dynamicFriction) * 0.5f;

            // Calculate from new velocities
            ic::Vec2f relativeVelocity = vel2f - vel1f;
            ic::Vec2f tangent = relativeVelocity - normal * normal.dot(relativeVelocity);
            float length = tangent.len();
            float threshold = 0.005f;

            if (length < threshold) {
                continue;
            } else {
                tangent = tangent / length;
            }

            float jt = -relativeVelocity.dot(tangent) / (m1 + m2);

            ic::Vec2f impulse;
            if (fabsf(jt) <= j * staticFriction) impulse = tangent * jt;
            else impulse = tangent * j * dynamicFriction * -1;

            if (object1->dynamic) object1->velocity = object1->velocity - impulse * m1;
            if (object2->dynamic) object2->velocity = object2->velocity + impulse * m2;
        }
    }
}