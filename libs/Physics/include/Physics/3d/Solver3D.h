#ifndef IC_PHYSICS_SOLVER_3D_H
#define IC_PHYSICS_SOLVER_3D_H

#include <vector>
#include <Physics/3d/Manifold3D.h>


namespace ic { namespace Physics {
    struct Solver3D {
        virtual void solve(const std::vector<ic::Physics::Manifold3D> &manifolds) {}
    };

    struct PositionSolver3D : Solver3D {
        void solve(const std::vector<ic::Physics::Manifold3D> &manifolds) override;
    };

    struct ImpulseSolver3D : Solver3D {
        void solve(const std::vector<ic::Physics::Manifold3D> &manifolds) override;
    };
}}

#endif