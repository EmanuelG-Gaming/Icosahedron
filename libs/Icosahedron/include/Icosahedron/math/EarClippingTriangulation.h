#ifndef IC_EAR_CLIPPING_TRIANGULATION_H
#define IC_EAR_CLIPPING_TRIANGULATION_H

#include <vector>
#include <stdexcept>

#include <Icosahedron/math/geom/Vectors.h>

namespace ic {
    namespace EarClippingTriangulation {
        enum class WindingOrder {
            invalid,

            clockwise,
            counterClockwise,
        };

        template <typename T>
        T get_item_looped(std::vector<T> &source, int index);

        template <typename T>
        std::vector<T> swap_vector(std::vector<T> &source);
        

        bool collinear(ic::Vec2f p1, ic::Vec2f p2, ic::Vec2f p3);
        bool has_collinear_sides(std::vector<ic::Vec2f> &polygon);


        bool simple_polygon(std::vector<ic::Vec2f> &polygon);

        bool point_inside_triangle(ic::Vec2f &point, ic::Vec2f A, ic::Vec2f B, ic::Vec2f C);

        float signed_area(std::vector<ic::Vec2f> &polygon);
        WindingOrder winding_from_area(float signedArea);
        
        std::vector<unsigned int> triangulate(std::vector<ic::Vec2f> &polygon);
        std::vector<unsigned int> triangulate(std::vector<float> &polygon);
    };
}
#endif