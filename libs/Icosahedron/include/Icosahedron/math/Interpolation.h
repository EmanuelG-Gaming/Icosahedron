#ifndef IC_INTERPOLATION_H
#define IC_INTERPOLATION_H


#include <cmath>

namespace ic {
    /** @brief Interpolation shaping functions.
     * 
     *  @note
     *  Inputs to these functions usually range from 0 to 1, and the outputs
     *  start from 0. */
    namespace Interpolation {
        float linear(float x);
        float quadratic(float x);
        float cubic(float x);
        float quartic(float x);
        float quintic(float x);

        float square_root(float x);

        /** @brief Provides a shaping function for the exponential e^x. 
         *  @note This is an approximation, as it reaches about 0.952 when x = 1. */
        float exponential(float x);

        float smoothstep(float x);
        float smootherstep(float x);
    };
}
#endif