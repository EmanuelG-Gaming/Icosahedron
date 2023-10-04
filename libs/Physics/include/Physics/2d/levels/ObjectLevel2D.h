#ifndef IC_PHYSICS_OBJECT_LEVEL_2D_H
#define IC_PHYSICS_OBJECT_LEVEL_2D_H

#include <vector>
#include <Physics/2d/objects/Object2D.h>
#include <Physics/2d/Manifold2D.h>
#include <Physics/2d/Solver2D.h>


namespace ic { namespace Physics {
    class ObjectLevel2D {
        public:
            virtual void load() {}
            virtual void update(float timeTook) {}
            
            void add_object(ic::Physics::Object2D *object);
            void add_solver(ic::Physics::Solver2D *solver);
            
            void send_collision_callbacks(const std::vector<ic::Physics::Manifold2D> &collisions, float timeTook);
            
            void resolve_collisions(float timeTook);
            
            std::vector<ic::Physics::Object2D*> &get_objects();
        
        protected:
            std::vector<ic::Physics::Object2D*> objects;
            std::vector<ic::Physics::Solver2D*> solvers;
            std::vector<ic::Physics::Manifold2D> manifolds, triggers;
            
            int lastIndex = 0;
    };
}}

#endif