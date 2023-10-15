#ifndef IC_PHYSICS_SPRING_MASS_SYSTEM_2D_H
#define IC_PHYSICS_SPRING_MASS_SYSTEM_2D_H

#include <vector>

#include <Icosahedron/math/geom/Spring2D.h>
#include <Physics/2d/objects/Object2D.h>


namespace ic { namespace Physics {
    class SpringMassSystem2D : public Object2D {
        public:
            /** @brief The mass of each individual vertex. */
            float pointMass;

            /** @brief Replaces the right side of the PV = nRT equation with a constant. */
            float pressureConstant;

            /** @brief Material properties of the springs. */
            float damping, stiffness;

            SpringMassSystem2D(float damping, float stiffness, float pressureConstant);

            void add_vertex(ic::Vec2f &position);
            void add_vertex(float x, float y);

            /** @note This would remove the last spring if the polygon is opened. */
            void set_closed(bool to);


            /** @returns The area contained in this shape. */
            float area();

            std::vector<ic::Vec2f> &get_vertex_positions();
            std::vector<ic::Vec2f> &get_vertex_velocities();
            std::vector<ic::Vec2f> &get_vertex_forces();
        
        protected:
            /** @brief Whether or not the springs form a closed polygon.
             *  If this is false, the result is a spring chain, thus it cannot be pressurised from the inside. 
            */
            bool isClosed;

            std::vector<ic::Vec2f> positions;
            std::vector<ic::Vec2f> velocities;
            std::vector<ic::Vec2f> forces;

            std::vector<ic::Spring2D*> springs;
    };
}}
#endif