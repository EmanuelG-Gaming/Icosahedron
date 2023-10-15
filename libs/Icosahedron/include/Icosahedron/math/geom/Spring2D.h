#ifndef IC_SPRING_2D_H
#define IC_SPRING_2D_H

#include <Icosahedron/math/geom/Vectors.h>


namespace ic {
    struct Spring2D {
        float damping = 0.0f;
        float stiffness = 0.0f;
        float restLength = 0.0f;

        ic::Vec2f force1, force2;
        ic::Vec2f normal;

        void update(ic::Vec2f position1, ic::Vec2f position2, ic::Vec2f velocity1, ic::Vec2f velocity2) {
            ic::Vec2f difference = position2 - position1;
            float length2 = difference.len2();

            if (length2 < 0.01f) return;

            float length = sqrt(length2);
            float ax = (velocity2.x() - velocity1.x()) * (position2.x() - position1.x());
            float ay = (velocity2.y() - velocity1.y()) * (position2.y() - position1.y());

            float magnitude = (length - restLength) * stiffness;
            magnitude += (ax + ay) * damping / length;

            ic::Vec2f springForce = difference.nor();
            normal = springForce.perpendicular(-1);
            springForce = springForce * magnitude;

            force1 = springForce;
            force2 = springForce * -1;
        }
    };
}

#endif