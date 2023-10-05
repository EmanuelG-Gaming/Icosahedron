#ifndef IC_PHYSICS_PHYSICS_LEVEL_2D_H
#define IC_PHYSICS_PHYSICS_LEVEL_2D_H

#include <thread>
#include <mutex>
#include <vector>

#include <Icosahedron/math/geom/Vectors.h>

#include <Physics/2d/levels/ObjectLevel2D.h>
#include <Physics/2d/objects/RigidObject2D.h>
#include <Physics/2d/Solver2D.h>


namespace ic { namespace Physics {
    class PhysicsLevel2D : public ObjectLevel2D {
        public:
            ic::Vec2f gravity;
            int simulationSteps;

            PhysicsLevel2D();

            void load() override;
            
            void set_fixed_time_length(int framesPerSecond);

            void set_gravity(float x, float y);
            void set_gravity(ic::Vec2f &acceleration);

            void dispose();

        private:
            void update_thread();
            void update_with_sub_steps(float timeTook);

        private:
            float timeAccumulator;
            float fixedTimeLength;
            uint32_t lastUpdate;

            std::vector<std::thread> physicsSimulationThreads;
            std::mutex physicsMutex;
            bool disabled;
    };
}}

#endif