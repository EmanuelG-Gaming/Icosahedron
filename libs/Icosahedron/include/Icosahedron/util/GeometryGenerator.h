#ifndef IC_GEOMETRY_GENERATOR_H
#define IC_GEOMETRY_GENERATOR_H

#include <vector>


#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/geom/Polygon.h>

#include <Icosahedron/math/Mathf.h>

namespace ic {
    /** @brief Utilities for procedurally loading geometry. */
    namespace GeometryGenerator {
        ic::Polygon generate_regular_polygon(int sides = 3, float radius = 1.0f) {
            std::vector<float> vertices;
            float angle = 0.0f;

            for (int i = 0; i < sides; i++) {
                angle += (2 * ic::Mathf::get().pi / sides);
                float x = ic::Mathf::get().cosf(angle) * radius;
                float y = ic::Mathf::get().sinf(angle) * radius;

                vertices.push_back(x);
                vertices.push_back(y);
            }

            ic::Polygon result = ic::Polygon(vertices);

            return result;
        }
    }
}
#endif