#ifndef IC_BEZIER_H
#define IC_BEIER_H

#include <Icosahedron/math/geom/Vectors.h>


namespace ic {
    namespace Bezier {
        /** @returns A point along a parametric, quadratic Bezier curve. */
        template<std::size_t dims>
        ic::Vector<float, dims> quadratic(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, ic::Vector<float, dims> &P3, float t);

        /** @returns each of the slopes of a quadratic Bezier curve on the X, Y, Z... axes, encoded in a vector. */
        template<std::size_t dims>
        ic::Vector<float, dims> quadratic_derivative(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, ic::Vector<float, dims> &P3, float t);

        ic::Vec2 quadratic2(ic::Vec2 &P1, ic::Vec2 &P2, ic::Vec2 &P3, float t);
        ic::Vec3 quadratic3(ic::Vec3 &P1, ic::Vec3 &P2, ic::Vec3 &P3, float t); 

        ic::Vec2 quadratic_derivative2(ic::Vec2 &P1, ic::Vec2 &P2, ic::Vec2 &P3, float t);
        ic::Vec3 quadratic_derivative3(ic::Vec3 &P1, ic::Vec3 &P2, ic::Vec3 &P3, float t); 
    };
}
#endif