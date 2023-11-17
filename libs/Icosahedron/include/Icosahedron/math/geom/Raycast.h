#ifndef IC_RAYCAST_H
#define IC_RAYCAST_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/Mathf.h>


namespace ic {
    class Raycast {
        public:
            static Raycast& get() {
                static Raycast ins;
                return ins;
            }

            bool line_segments_collide(float x1, float y1, float x2, float y2,
                                        float x3, float y3, float x4, float y4) {

                float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

                bool linesParallelOrCoincident = denominator == 0.0f;
                if (linesParallelOrCoincident) {
                    return false;
                }

                float t = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
                float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3));

                t /= denominator;
                u /= denominator;

                bool intersects = (t >= 0.0f && t <= 1.0f) && (u >= 0.0f && u <= 1.0f);
                return intersects; 
            }

            ic::Vec2f get_closest_point(ic::Vec2f &point, ic::Vec2f sidePointA, ic::Vec2f sidePointB) {
                ic::Vec2f gradientLine = sidePointB - sidePointA;
                ic::Vec2f gradientPoint = point - sidePointA;

                float t = gradientLine.dot(gradientPoint) / gradientLine.len2();
                t = ic::Mathf::get().clamp(t, 0.0f, 1.0f);

                return sidePointA + gradientLine * t;
            }

        private:
            Raycast() {}
            ~Raycast() {}

        public:
            Raycast(Raycast const&) = delete;
            void operator = (Raycast const&) = delete;
    };
}

#endif