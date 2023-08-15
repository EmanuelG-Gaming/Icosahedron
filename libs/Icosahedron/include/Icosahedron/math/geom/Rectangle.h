#ifndef IC_RECTANGLE_H
#define IC_RECTANGLE_H

#include <Icosahedron/math/geom/Vectors.h>

namespace ic {
    /** @brief A rectangle can be defined with a center position and a size (half-width and half-height).
     *  It is the two-dimensional analog of the more generalized axis-aligned bounding box (AABB). */
    struct Rectangle {
        ic::Vec2f position;
        ic::Vec2f size;

        Rectangle() {}
        Rectangle(ic::Vec2f position, ic::Vec2f size) : position(position), size(size) {}

        bool contains(ic::Vec2f &point) {
            float x1 = position.x() - size.x(), y1 = position.y() - size.y();
            float x2 = position.x() + size.x(), y2 = position.y() + size.y();

            return (point.x() >= x1 && point.x() <= x2) &&
                   (point.y() >= y1 && point.y() <= y2);
        }

        bool overlaps(ic::Rectangle &other) {
            float x1 = position.x() - size.x(), y1 = position.y() - size.y();
            float x2 = position.x() + size.x(), y2 = position.y() + size.y();

            float x3 = other.position.x() - other.size.x(), y3 = other.position.y() - other.size.y();
            float x4 = other.position.x() + other.size.x(), y4 = other.position.y() + other.size.y();

            return (x2 >= x3 && x1 <= x4) &&
                   (y2 >= y3 && y1 <= y4);
        }

        /** @returns The width and height of an overlap of two rectangles. */
        ic::Vec2f find_overlap(ic::Rectangle &other) {
            float x1 = position.x() - size.x(), y1 = position.y() - size.y();
            float x2 = position.x() + size.x(), y2 = position.y() + size.y();

            float x3 = other.position.x() - other.size.x(), y3 = other.position.y() - other.size.y();
            float x4 = other.position.x() + other.size.x(), y4 = other.position.y() + other.size.y();

            ic::Vec2f overlap;
            overlap.x() = x2 > x4 ? x4 - x1 : x2 - x3;
            overlap.y() = y2 > y4 ? y4 - y1 : y2 - y3;

            return overlap;
        }
    };
}
#endif