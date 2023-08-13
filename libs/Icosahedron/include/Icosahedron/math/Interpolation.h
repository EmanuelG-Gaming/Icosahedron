#ifndef IC_INTERPOLATION_H
#define IC_INTERPOLATION_H

#include <cmath>

namespace ic {
    /* Interpolation shaping functions.
     * Note that inputs usually range from 0 to 1, and the outputs
     * start from 0. */
    class Interpolation {
        public:
            static Interpolation& get() {
                static Interpolation ins;
                return ins;
            }

            float linear(float x) {
                return x;
            }
            float quadratic(float x) {
                return x * x;
            }
            float cubic(float x) {
                return x * x * x;
            }
            float quartic(float x) {
                return x * x * x * x;
            }
            float quintic(float x) {
                return x * x * x * x * x;
            }

            /* Provides a shaping function for the exponential e^x. 
             * Note that this is an approximation, as it reaches about 0.982 when x = 1. */
            float exponential(float x) {
                float scale = 4.0f / 7.0f;
                return (exp(x) - 1) * scale;
            }

            float smoothstep(float x) {
                return (3.0f - x * 2.0f) * x * x;
            }

        private:
            Interpolation() {}
            ~Interpolation() {}

        public:
            Interpolation(Interpolation const&) = delete;
            void operator = (Interpolation const&) = delete;
    };
}
#endif