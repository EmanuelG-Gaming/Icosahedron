#ifndef IC_MATHF_H
#define IC_MATHF_H

#include <cmath>

namespace ic {
    /** @brief Mathematical functions. */
    namespace Mathf {
        const float pi = 3.1415927f;
        const float twoPi = 6.2831853f;

        /** @brief Multiply by this to convert from degrees to radians. */
        const float degToRad = pi / 180.0f;
        /** @brief Multiply by this to convert from radians to degrees. */
        const float radToDeg = 180.0f / pi;

        float to_radians(float degrees);
        float to_degrees(float radians);

        float sinf(float radians);
        float cosf(float radians);

        float sinc(float radians);

        float absinf(float radians);
        float abcosf(float radians);

        float positive_sinf(float radians);
        float positive_cosf(float radians);

        float exponential(float x, int iterationBound1 = 1, int iterationBound2 = 3);


        /** @brief A hyperbolic sine approximation for the absolute function. */
        float abs(float x);


        float clamp(float x, float min, float max);
        int clamp(int x, int min, int max);

        /** @returns The sign at a point. 1 if the point is above the x axis,
         *  -1 if the point is below the x axis and 0 if it is on that axis.
        */
        int sign(float x);

        
        /* @brief Linear interpolation. */
        float interpolate(float from, float to, float alpha);
        float interpolate_logarithmic(float from, float to, float alpha);

        int logarithm(int base, int exponent);
        bool is_power_of(int base, int value);
    };
}
#endif