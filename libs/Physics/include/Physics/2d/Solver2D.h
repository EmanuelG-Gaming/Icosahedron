#ifndef IC_PHYSICS_SOLVER_2D_H
#define IC_PHYSICS_SOLVER_2D_H

#include <vector>
#include <Physics/2d/Manifold2D.h>


namespace ic { namespace Physics {
    struct Solver2D {
        virtual void solve(const std::vector<ic::Physics::Manifold2D> &manifolds) {}
    };

    struct PositionSolver2D : Solver2D {
        void solve(const std::vector<ic::Physics::Manifold2D> &manifolds);
    };

    struct ImpulseSolver2D : Solver2D {
        void solve(const std::vector<ic::Physics::Manifold2D> &manifolds);
    };
}}

#endif