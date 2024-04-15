#include <Physics/Solver.h>
#include <Physics/objects/RigidObject.h>

void ic::Physics::PositionSolver::solve(const std::vector<ic::Physics::Manifold> &manifolds) {
    for (auto &manifold : manifolds) {
        ic::Physics::Object *objA = manifold.object1;
        ic::Physics::Object *objB = manifold.object2;

        int immovable1 = (int) !objA->dynamic;
        int immovable2 = (int) !objB->dynamic;
        
        ic::Vec3f normal = manifold.points.normal;
        ic::Vec3f displacement = normal * (1.0f / (float) std::max(1, immovable1 + immovable2)) * manifold.points.depth * 0.5f;

        objA->transform->position = objA->transform->position - displacement * (1 - immovable1);
        objB->transform->position = objB->transform->position + displacement * (1 - immovable2);
    }
}

void ic::Physics::ImpulseSolver::solve(const std::vector<ic::Physics::Manifold> &manifolds) {
    for (auto &manifold : manifolds) {
        ic::Physics::RigidObject *object1 = dynamic_cast<ic::Physics::RigidObject*>(manifold.object1);
        ic::Physics::RigidObject *object2 = dynamic_cast<ic::Physics::RigidObject*>(manifold.object2);
        if (object1 == nullptr || object2 == nullptr) continue;
        
        ic::Vec3f vel1 = object1->dynamic ? object1->velocity : object2->velocity * -1.0f;
        ic::Vec3f vel2 = object2->dynamic ? object2->velocity : object1->velocity * -1.0f;
        ic::Vec3f vel1f = object1->velocity;
        ic::Vec3f vel2f = object2->velocity;

        ic::Vec3f normal = manifold.points.normal;

        ic::Vec3f gradientVelocity = vel2 - vel1;
        ic::Vec3f gradientVelocityF = vel2f - vel1f;
        
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
            ic::Vec3f reflectionDisplacement = normal * j;
            if (object1->dynamic) object1->velocity = object1->velocity - reflectionDisplacement * m2;
            if (object2->dynamic) object2->velocity = object2->velocity + reflectionDisplacement * m1;
        }

        // Object friction
        {
            float staticFriction =  (object1->staticFriction + object2->staticFriction)   * 0.5f;
            float dynamicFriction = (object1->dynamicFriction + object2->dynamicFriction) * 0.5f;

            // Calculate from new velocities
            ic::Vec3f relativeVelocity = vel2f - vel1f;
            ic::Vec3f tangent = relativeVelocity - normal * normal.dot(relativeVelocity);
            float length = tangent.len();
            float threshold = 0.005f;

            if (length < threshold) {
                continue;
            } else {
                tangent = tangent / length;
            }

            float jt = -relativeVelocity.dot(tangent) / (m1 + m2);

            ic::Vec3f impulse;
            if (fabsf(jt) <= j * staticFriction) impulse = tangent * jt;
            else impulse = tangent * j * dynamicFriction * -1;

            if (object1->dynamic) object1->velocity = object1->velocity - impulse * m1;
            if (object2->dynamic) object2->velocity = object2->velocity + impulse * m2;
        }
    }
}