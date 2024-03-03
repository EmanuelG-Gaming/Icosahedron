#ifndef IC_RASTERIZATION_H
#define IC_RASTERIZATION_H

#include <functional>

#include <Icosahedron/math/geom/Vectors.h>


namespace ic {
    /** @brief Rasterization is the process of converting geometric data (shapes) into pixels. */
    namespace Rasterization {
        namespace {
            void fill_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, const std::function<void(int, int)> &callback);
            void fill_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, const std::function<void(int, int)> &callback);
        }

        
        /** @brief Acts on a function of integer (x, y) pairs, where each point belongs to
         *  a Bresenham-rendered line segment. */
        void line(int x0, int y0, int x1, int y1, const std::function<void(int, int)> &callback);

        void triangle(int x0, int y0, int x1, int y1, int x2, int y2, const std::function<void(int, int)> &with);
    };
}
#endif