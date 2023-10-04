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
            int simulationSteps;

            PhysicsLevel2D();

            void load() override;
            void update(float timeTook) override;

        private:
            void update_with_sub_steps(float timeTook);
    };
}}

#endif