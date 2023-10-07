#ifndef IC_PHYSICS_PHYSICS_LEVEL_2D_H
#define IC_PHYSICS_PHYSICS_LEVEL_2D_H

#include <vector>

#include <Icosahedron/math/geom/Vectors.h>

#include <Physics/2d/objects/RigidObject2D.h>
#include <Physics/2d/Solver2D.h>


namespace ic { namespace Physics {
    class PhysicsLevel2D {
        public:
            ic::Vec2f gravity;
            int simulationSteps;

            PhysicsLevel2D();

            void load();
            
            void update(float timeTook);

            
            void set_fixed_time_length(int framesPerSecond);
            void set_gravity(float x, float y);
            void set_gravity(ic::Vec2f &acceleration);

            void add_object(ic::Physics::Object2D *object);
            ic::Physics::Object2D *get_object(int index);
            
            std::vector<ic::Physics::Object2D*> &get_objects();
        
        private:
            void update_with_sub_steps(float timeTook);

            void send_collision_callbacks(const std::vector<ic::Physics::Manifold2D> &collisions, float timeTook);
            void resolve_collisions(float timeTook);


            void add_solver(ic::Physics::Solver2D *solver);
            
        private:
            float timeAccumulator;
            float fixedTimeLength;

            std::vector<ic::Physics::Object2D*> objects;
            std::vector<ic::Physics::Solver2D*> solvers;
            std::vector<ic::Physics::Manifold2D> manifolds, triggers;
    };
}}

#endif