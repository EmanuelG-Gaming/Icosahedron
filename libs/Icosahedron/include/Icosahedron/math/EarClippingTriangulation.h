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
        T get_item_looped(const std::vector<T> &source, int index) {
            int length = source.size();
            if (index >= length) {
                return source[index % length];
            } else if (index < 0) {
                return source[index % length + length];
            }

            return source[index];
        }

        template <typename T>
        std::vector<T> swap_vector(const std::vector<T> source) {
            for (int i = 0; i < source.size() / 2; i++) {
                std::swap(source[i], source[source.size() - i - 1]);
            }
            return source;
        }

        bool collinear(ic::Vec2f p1, ic::Vec2f p2, ic::Vec2f p3) {
            float determinant = p1.x() * (p2.y() - p3.y()) +
                                p2.x() * (p3.y() - p1.y()) +
                                p3.x() * (p1.y() - p2.y()) ;

            return (determinant == 0.0f);
        }
        bool has_collinear_sides(const std::vector<ic::Vec2f> &polygon) {
            for (int i = 0; i < polygon.size(); i++) {
                ic::Vec2f B = get_item_looped(polygon, i - 1);
                ic::Vec2f A = polygon[i];
                ic::Vec2f C = get_item_looped(polygon, i + 1);

                if (collinear(A, B, C)) {
                    return true;
                }
            }

            return false;
        }


        bool simple_polygon(std::vector<ic::Vec2f> &polygon) {
            return true;
        }

        bool point_inside_triangle(ic::Vec2f &point, ic::Vec2f A, ic::Vec2f B, ic::Vec2f C) {
            ic::Vec2f BA = B - A;
            ic::Vec2f CB = C - B;
            ic::Vec2f AC = A - C;

            ic::Vec2f PA = point - A;
            ic::Vec2f PB = point - B;
            ic::Vec2f PC = point - C;

            if (BA.crs(PA) > 0.0f ||
                CB.crs(PB) > 0.0f ||
                AC.crs(PC) > 0.0f) {
                    return false;
            }

            return true;
        }

        float signed_area(std::vector<ic::Vec2f> &polygon) {
            float result = 0.0f;
            for (int i = 0; i < polygon.size() - 1; i++) {
                ic::Vec2f point1 = polygon[i];
                ic::Vec2f point2 = polygon[i + 1];

                result += (point1.x() * point2.y() - point2.x() * point1.y());
            }
            result /= 2.0f;

            return result;
        }

        WindingOrder winding_from_area(float signedArea) {
            if (signedArea < 0.0f) {
                return WindingOrder::clockwise;
            } else if (signedArea > 0.0f) {
                return WindingOrder::counterClockwise;
            }

            return WindingOrder::invalid;
        }

        std::vector<int> triangulate(std::vector<ic::Vec2f> &polygon) {
            if (!simple_polygon(polygon)) {
                throw std::runtime_error("Polygon is not simple!");
            }
            if (has_collinear_sides(polygon)) {
                throw std::runtime_error("Polygon has a collinear side or more.");
            }

            float area = signed_area(polygon);
            WindingOrder winding = winding_from_area(area);

            if (winding == WindingOrder::invalid) {
                throw std::runtime_error("Couldn't triangulate a singular polygon.");
            }
            if (winding == WindingOrder::counterClockwise) {
                swap_vector(polygon);
                printf("Winding order of polygon is counter-clockwise.");
            }


            int totalTriangleCount = polygon.size() - 2;
            int totalIndexCount = totalTriangleCount * 3;

            std::vector<int> indices, triangles;
            for (int i = 0; i < polygon.size(); i++) {
                indices.push_back(i);
            }
            triangles.resize(totalIndexCount);


            int indexCount = 0;
            while (indices.size() > 3) {
                for (int i = 0; i < indices.size(); i++) {
                    int A = indices[i];
                    int B = get_item_looped(indices, i - 1);
                    int C = get_item_looped(indices, i + 1);

                    ic::Vec2f vecA = polygon[A];
                    ic::Vec2f vecB = polygon[B];
                    ic::Vec2f vecC = polygon[C];

                    ic::Vec2f BA = vecB - vecA;
                    ic::Vec2f CA = vecC - vecA;

                    if (BA.crs(CA) < 0.0f) {
                        continue;
                    }

                    bool isEar = true;
                    for (int j = 0; j < polygon.size(); j++) {
                        if (j == A || j == B || j == C) {
                            continue;
                        }

                        ic::Vec2f point = polygon[j];
                        if (point_inside_triangle(point, vecA, vecB, vecC)) {
                            isEar = false;
                            break;
                        }
                    }

                    if (isEar) {
                        triangles[indexCount++] = B;
                        triangles[indexCount++] = A;
                        triangles[indexCount++] = C;

                        indices.erase(indices.begin() + i);
                        break;
                    }
                }
            }

            triangles[indexCount++] = indices[0];
            triangles[indexCount++] = indices[1];
            triangles[indexCount++] = indices[2];

            return triangles;
        }
    }
}
#endif