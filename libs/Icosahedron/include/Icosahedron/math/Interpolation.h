#ifndef IC_INTERPOLATION_H
#define IC_INTERPOLATION_H

#include <cmath>

namespace ic {
    /** @brief Interpolation shaping functions.
     * 
     *  @note
     *  Inputs to these functions usually range from 0 to 1, and the outputs
     *  start from 0. */
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
            
            float square_root(float x) {
                return sqrt(x);
            }

            /** @brief Provides a shaping function for the exponential e^x. 
             *  @note This is an approximation, as it reaches about 0.952 when x = 1. */
            float exponential(float x) {
                float sum = 0.0f;
                int factorial = 1;
                
                // Calculate power series
                for (int i = 1; i <= 3; i++) {
                    factorial *= i;
                    sum += pow(x, i) / (float) factorial;
                }

                return sum * (4.0f / 7.0f);
            }
            
            float smoothstep(float x) {
                return (3.0f - x * 2.0f) * x * x;
            }

            float smootherstep(float x) {
                return x * x * x * (x * (6.0f * x - 15.0f) + 10.0f);
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