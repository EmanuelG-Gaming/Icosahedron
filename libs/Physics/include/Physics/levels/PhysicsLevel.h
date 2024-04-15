#ifndef IC_PHYSICS_PHYSICS_LEVEL_H
#define IC_PHYSICS_PHYSICS_LEVEL_H

#include <vector>

#include <Icosahedron/math/geom/Vectors.h>

#include <Physics/objects/RigidObject.h>
#include <Physics/Solver.h>
#include <Physics/forces/Force.h>
#include <Physics/forces/Gravity.h>


namespace ic { namespace Physics {
    class PhysicsLevel {
        public:
            int simulationSteps;

            PhysicsLevel();

            void load();
            
            void update(float timeTook);

            
            void set_fixed_time_length(int framesPerSecond);
            void set_gravity(float x, float y, float z = 0.0f);
            void set_gravity(ic::Vec3f &force);

            void add_object(ic::Physics::Object *object);
            void remove_object(ic::Physics::Object *object);

            ic::Physics::Object *get_object(int index);
            std::vector<ic::Physics::Object*> &get_objects();
            
            void add_force(ic::Physics::Force *force);
            void reset_forces();
            
        private:
            void update_with_sub_steps(float timeTook);

            void send_collision_callbacks(const std::vector<ic::Physics::Manifold> &collisions, float timeTook);
            void resolve_collisions(float timeTook);


            void add_solver(ic::Physics::Solver *solver);
            
        private:
            float timeAccumulator;
            float fixedTimeLength;
            bool shouldResetForces;

            ic::Physics::Gravity *gravity;

            std::vector<ic::Physics::Object*> objects;
            std::vector<ic::Physics::Solver*> solvers;
            std::vector<ic::Physics::Force*> forces;

            std::vector<ic::Physics::Manifold> manifolds, triggers;
    };
}}

#endif