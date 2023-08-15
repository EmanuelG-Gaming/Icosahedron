#ifndef IC_MATHF_H
#define IC_MATHF_H

#include <cmath>

namespace ic {
    /** @brief Mathematical functions. */
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

            float absinf(float radians) {
                return abs(sinf(radians));
            }
            float abcosf(float radians) {
                return abs(cosf(radians));
            }

            float positive_sinf(float radians) {
                return (sinf(radians) + 1) * 0.5f;
            }
            float positive_cosf(float radians) {
                return (cosf(radians) + 1) * 0.5f;
            }


            float clamp(float x, float min, float max) {
                return std::max(min, std::min(x, max));
            }

            /* Linear interpolation. */
            float interpolate(float from, float to, float alpha) {
                // pt = p1 + (p2 - p1) * t
                // pt = p1 + p2t - p1t
                // pt = p1 * (1 - t) + p2t

                return from * (1 - alpha) + to * alpha;
            }
            float interpolate_logarithmic(float from, float to, float alpha) {
                float value = interpolate(log(from), log(to), alpha);
                value = exp(value);

                return value;
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