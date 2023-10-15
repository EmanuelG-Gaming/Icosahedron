#ifndef IC_PHYSICS_COLLISION_UTILS_2D_H
#define IC_PHYSICS_COLLISION_UTILS_2D_H

#include <Physics/2d/ManifoldPoints2D.h>
#include <Physics/2d/Collider2D.h>
#include <Physics/2d/Transform2D.h>

namespace ic { namespace Physics {
    class CollisionUtils2D {
        public:
            static CollisionUtils2D& get() {
                static CollisionUtils2D ins;
                return ins;
            }
            
            ManifoldPoints2D circle_circle(CircleCollider *colliderA, Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB);
            ManifoldPoints2D rectangle_circle(RectangleCollider *colliderA, Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB);
            //ManifoldPoints2D spMass2D_circle(SpringMassCollider2D *colliderA, Transform2D *transformA, CircleCollider *colliderB, Transform2D *transformB);
            //ManifoldPoints2D spMass2D_rectangle(SpringMassCollider2D *colliderA, Transform2D *transformA, RectangleCollider *colliderB, Transform2D *transformB);
            
        private:
            CollisionUtils2D() {}
            ~CollisionUtils2D() {}
    
        public:
            CollisionUtils2D(CollisionUtils2D const&) = delete;
            void operator = (CollisionUtils2D const&) = delete;
    };
}}
#endif