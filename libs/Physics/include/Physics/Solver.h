#ifndef IC_PHYSICS_SOLVER_H
#define IC_PHYSICS_SOLVER_H

#include <vector>
#include <Physics/Manifold.h>


namespace ic { namespace Physics {
    struct Solver {
        virtual void solve(const std::vector<ic::Physics::Manifold> &manifolds) {}
    };

    struct PositionSolver : Solver {
        void solve(const std::vector<ic::Physics::Manifold> &manifolds) override;
    };

    struct ImpulseSolver : Solver {
        void solve(const std::vector<ic::Physics::Manifold> &manifolds) override;
    };
}}

#endif