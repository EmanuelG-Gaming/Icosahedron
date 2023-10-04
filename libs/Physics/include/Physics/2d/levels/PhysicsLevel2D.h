#ifndef IC_PHYSICS_PHYSICS_LEVEL_2D_H
#define IC_PHYSICS_PHYSICS_LEVEL_2D_H

#include <Icosahedron/math/geom/Vectors.h>

#include <Physics/2d/levels/ObjectLevel2D.h>
#include <Physics/2d/objects/RigidObject2D.h>
#include <Physics/2d/Solver2D.h>


namespace ic { namespace Physics {
    class PhysicsLevel2D : public ObjectLevel2D {
        public:
            ic::Vec2f gravity;
            
            void load() override {
                simulationSteps = 5;
                gravity = { 0.0f, -9.81f };
                 
                objects.clear();
                solvers.clear();
                
                add_solver(new ic::Physics::PositionSolver2D());
                add_solver(new ic::Physics::ImpulseSolver2D());
            }
            
            void update(float timeTook) override {
                float stepSize = timeTook / (float)simulationSteps;
                for (int i = 0; i < simulationSteps; i++) {
                    update_with_sub_steps(stepSize);
                }
            }
            
            void update_with_sub_steps(float timeTook) {
                resolve_collisions(timeTook);
                
                // Reseting the forces + applying
                for (auto &object : objects) {
                    ic::Physics::RigidObject2D *body = dynamic_cast<ic::Physics::RigidObject2D*>(object);
                    if (body == nullptr) continue;
                    
                    body->force = { 0.0f, 0.0f };
                     
                    //for (auto &force : forces) {
                    //    force->apply(body, timeTook);
                    //}
                    ic::Vec2f acceleration = body->velocity * -1 + gravity;
                    body->force = body->force + acceleration * body->mass;
                }

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
            
        private:
            int simulationSteps;
    };
}}

#endif