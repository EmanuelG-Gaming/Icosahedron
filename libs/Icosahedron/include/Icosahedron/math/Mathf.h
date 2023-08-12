#ifndef IC_MATHF_H
#define IC_MATHF_H

#include <cmath>

namespace ic {
    /* Mathematical functions. */
    class Mathf {
        public:
            static Mathf& get() {
                static Mathf ins;
                return ins;
            }

            const float pi = 3.1415927f;
            /* Multiply by this to convert from degrees to radians. */
            const float degToRad = pi / 180.0f;
            /* Multiply by this to convert from radians to degrees. */
            const float radToDeg = 180.0f / pi;

            float to_radians(float degrees) {
                return degrees * degToRad;
            }
            float to_degrees(float radians) {
                return radians * radToDeg;
            }

            float sinf(float radians) {
                return sin(radians);
            }
            float cosf(float radians) {
                return cos(radians);
            }

        private:
            Mathf() {}
            ~Mathf() {}

        public:
            Mathf(Mathf const&) = delete;
            void operator = (Mathf const&) = delete;
    };
}
#endif