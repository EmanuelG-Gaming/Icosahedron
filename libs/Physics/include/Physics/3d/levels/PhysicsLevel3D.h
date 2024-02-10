#ifndef IC_PHYSICS_PHYSICS_LEVEL_3D_H
#define IC_PHYSICS_PHYSICS_LEVEL_3D_H

#include <vector>

#include <Icosahedron/math/geom/Vectors.h>

#include <Physics/3d/objects/RigidObject3D.h>

#include <Physics/3d/Solver3D.h>
#include <Physics/3d/forces/Force3D.h>
#include <Physics/3d/forces/Gravity3D.h>


namespace ic { namespace Physics {
    class PhysicsLevel3D {
        public:
            int simulationSteps;

            PhysicsLevel3D();

            void load();
            
            void update(float timeTook);

            
            void set_fixed_time_length(int framesPerSecond);
            void set_gravity(float x, float y, float z);
            void set_gravity(ic::Vec3f &force);

            void add_object(ic::Physics::Object3D *object);
            void remove_object(ic::Physics::Object3D *object);
            ic::Physics::Object3D *get_object(int index);
            std::vector<ic::Physics::Object3D*> &get_objects();
            
            void add_force(ic::Physics::Force3D *force);
            void reset_forces();
            
        private:
            void update_with_sub_steps(float timeTook);

            void send_collision_callbacks(const std::vector<ic::Physics::Manifold3D> &collisions, float timeTook);
            void resolve_collisions(float timeTook);


            void add_solver(ic::Physics::Solver3D *solver);
            
        private:
            float timeAccumulator;
            float fixedTimeLength;
            bool shouldResetForces;

            ic::Physics::Gravity3D *gravity;

            std::vector<ic::Physics::Object3D*> objects;
            std::vector<ic::Physics::Solver3D*> solvers;
            std::vector<ic::Physics::Force3D*> forces;

            std::vector<ic::Physics::Manifold3D> manifolds, triggers;
    };
}}

#endif