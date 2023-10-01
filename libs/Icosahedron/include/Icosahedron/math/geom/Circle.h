#ifndef IC_CIRCLE_H
#define IC_CIRCLE_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/Mathf.h>

namespace ic {
    struct Circle {
        float x, y;
        float radius;

        Circle() {
            x = y = radius = 0.0f;
        }

        Circle(float x, float y, float radius) : x(x), y(y), radius(radius) {}
        Circle(ic::Vec2f &position, float radius) : x(position.x()), y(position.y()), radius(radius) {}

        bool contains(Vec2f &point) {
            float dx = point.x() - x;
            float dy = point.y() - y;

            return (dx * dx + dy * dy) <= (radius * radius);
        }
        bool contains(const ic::Circle &circle) {
            // Check if one circle is bigger than the other
            float difference = radius - circle.radius;
            if (difference < 0.0f) return false;

            float dx = circle.x - x;
            float dy = circle.y - y;
            float dst2 = dx * dx + dy * dy;
            float sum = radius + circle.radius;

            return !(difference * difference < dst2) && (dst2 < sum * sum);
        }


        bool overlaps(const ic::Circle &circle) {
            float dx = circle.x - x;
            float dy = circle.y - y;
            float sum = radius + circle.radius;

            return (dx * dx + dy * dy) <= (sum * sum);
        }

        float area() {
            return radius * radius * ic::Mathf::get().pi;
        }

        float circumference() {
            return radius * ic::Mathf::get().twoPi;
        }

        friend std::ostream& operator<<(std::ostream &stream, ic::Circle &circle) {
            stream << "((" << circle.x << ", " << circle.y << "), " << circle.radius << ")";
            return stream;
        }
    };
}
#endif