#ifndef IC_BEZIER_H
#define IC_BEIER_H

#include <Icosahedron/math/geom/Vectors.h>


namespace ic {
    namespace Bezier {
        /** @note Refer to ic::Vector::interpolate(). */
        template<std::size_t dims>
        ic::Vector<float, dims> linear(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, float t);

        template<std::size_t dims>
        ic::Vector<float, dims> linear_derivative(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, float t);



        /** @returns A point along a parametric, quadratic Bezier curve. */
        template<std::size_t dims>
        ic::Vector<float, dims> quadratic(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, ic::Vector<float, dims> &P3, float t);

        /** @returns each of the slopes of a quadratic Bezier curve on the X, Y, Z... axes, encoded in a vector. */
        template<std::size_t dims>
        ic::Vector<float, dims> quadratic_derivative(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, ic::Vector<float, dims> &P3, float t);



        /** @returns A point along a parametric, cubic Bezier curve. */
        template<std::size_t dims>
        ic::Vector<float, dims> cubic(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, ic::Vector<float, dims> &P3, ic::Vector<float, dims> &P4, float t);

        /** @returns each of the slopes of a cubic Bezier curve on the X, Y, Z... axes, encoded in a vector. */
        template<std::size_t dims>
        ic::Vector<float, dims> cubic_derivative(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, ic::Vector<float, dims> &P3, ic::Vector<float, dims> &P4, float t);

        /** @brief The length of a Bezier curve has no clear closed-form solution, meaning that an approximate solution needs to be calculated. */
        template<std::size_t dims>
        float quadratic_length(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, ic::Vector<float, dims> &P3, int divisions = 50);

        template<std::size_t dims>
        float cubic_length(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, ic::Vector<float, dims> &P3, ic::Vector<float, dims> &P4, int divisions = 50);
    };
}
#endif