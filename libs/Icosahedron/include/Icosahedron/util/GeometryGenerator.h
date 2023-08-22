#ifndef IC_GEOMETRY_GENERATOR_H
#define IC_GEOMETRY_GENERATOR_H

#include <vector>


#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/geom/Polygon.h>

#include <Icosahedron/math/Mathf.h>

namespace ic {
    /** @brief Utilities for procedurally loading geometry. */
    class GeometryGenerator {
public:
        static GeometryGenerator& get() {
            static GeometryGenerator ins;
            return ins;
        }

        std::vector<float> generate_regular_polygon(int sides = 3, float radius = 1.0f) {
            std::vector<float> vertices;
            float angle = 0.0f;

            for (int i = 0; i < sides; i++) {
                angle += (2 * ic::Mathf::get().pi / sides);
                float x = ic::Mathf::get().cosf(angle) * radius;
                float y = ic::Mathf::get().sinf(angle) * radius;

                vertices.push_back(x);
                vertices.push_back(y);
            }

            return vertices;
        }

        std::vector<float> generate_rectangle(float width, float height) {
            std::vector<float> vertices = {
                -width, height,
                width, height,
                width, -height,
                -width, -height
            };

            return vertices;
        }
        
        std::vector<float> generate_UV_polygon(const std::vector<float> &verts, float magnification = 1.0f) {
            float width = 0, height = 0;

            float minX = verts[0];
            float minY = verts[1];
            float maxX = verts[0];
            float maxY = verts[1];

            for (int i = 0; i < verts.size(); i += 2) {
                float dx = verts[i];
                float dy = verts[i + 1];

                minX = minX > dx ? dx : minX;
                minY = minY > dy ? dy : minY;

                maxX = maxX < dx ? dx : maxX;
                maxY = maxY < dy ? dy : maxY;
            }

            width = maxX - minX;
            height = maxY - minY;

            std::vector<float> textureCoords;
            for (int i = 0; i < verts.size(); i += 2) {
                float x = verts[i], y = verts[i + 1];

                float nx = (x / width + 1) * (2.0f / magnification);
                float ny = (y / height + 1) * (2.0f / magnification);

                ny *= -1;
                textureCoords.push_back(nx);
                textureCoords.push_back(ny);
            }
            return textureCoords;
        }

private:
        GeometryGenerator() {}
        ~GeometryGenerator() {}
public:
        GeometryGenerator(GeometryGenerator const&) = delete;
        void operator = (GeometryGenerator const&) = delete;
    };
}
#endif