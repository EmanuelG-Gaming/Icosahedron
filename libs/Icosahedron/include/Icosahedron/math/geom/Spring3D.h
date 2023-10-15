#ifndef IC_SPRING_3D_H
#define IC_SPRING_3D_H

#include <Icosahedron/math/geom/Vectors.h>


namespace ic {
    struct Spring3D {
        float damping = 0.0f;
        float stiffness = 0.0f;
        float restLength = 0.0f;

        ic::Vec3f force1, force2;
        
        void update(ic::Vec3f position1, ic::Vec3f position2, ic::Vec3f velocity1, ic::Vec3f velocity2) {
            ic::Vec3f difference = position2 - position1;
            float length2 = difference.len2();

            if (length2 < 0.01f) return;

            float length = sqrt(length2);
            float ax = (velocity2.x() - velocity1.x()) * (position2.x() - position1.x());
            float ay = (velocity2.y() - velocity1.y()) * (position2.y() - position1.y());
            float az = (velocity2.z() - velocity1.z()) * (position2.z() - position1.z());

            float magnitude = (length - restLength) * stiffness;
            magnitude += (ax + ay + az) * damping / length;

            ic::Vec3f springForce = difference.nor();
            springForce = springForce * magnitude;

            force1 = springForce;
            force2 = springForce * -1;
        }
    };
}

#endif