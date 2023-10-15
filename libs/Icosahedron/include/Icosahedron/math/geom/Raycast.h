#ifndef IC_RAYCAST_H
#define IC_RAYCAST_H

#include <Icosahedron/math/geom/Vectors.h>


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

        private:
            Raycast() {}
            ~Raycast() {}

        public:
            Raycast(Raycast const&) = delete;
            void operator = (Raycast const&) = delete;
    };
}

#endif