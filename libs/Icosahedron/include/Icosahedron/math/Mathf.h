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


            void load() {
                for (int i = 0; i < 360; i++) {
                    float angle = to_radians(i);
                    sineValues[i] = sin(angle);
                    cosineValues[i] = cos(angle);
                }
            }

            float sinf(float radians) {
                float r = radians;
                if (r < 0) r += 2 * pi;
                if (r > 2 * pi) r -= 2 * pi;
                
                float degrees = to_degrees(r);
                return sineValues[(int) degrees];
            }
            float cosf(float radians) {
                float r = radians;
                if (r < 0) r += 2 * pi;
                if (r > 2 * pi) r -= 2 * pi;

                float degrees = to_degrees(r);
                return cosineValues[(int) degrees];
            }

        private:
            Mathf() {}
            ~Mathf() {}

        public:
            Mathf(Mathf const&) = delete;
            void operator = (Mathf const&) = delete;

        private:
            /* Trigonometric function lookup table. */
            float sineValues[360], cosineValues[360];
    };
}
#endif