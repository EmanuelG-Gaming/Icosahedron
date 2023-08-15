#ifndef IC_POLYGON_H
#define IC_POLYGON_H

#include <vector>
#include <initializer_list>
#include <stdexcept>

#include <Icosahedron/math/Mathf.h>
#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/geom/Rectangle.h>

namespace ic {
    /** @brief Illustrates a polygon with local and transformed vertices in two-dimensional space. */
    struct Polygon {
        public:
            /** @brief Initializes a new, but empty polygon. */
            Polygon() {
                this->localVertices.reserve(0);
            }

            /** @brief Constructs a polygon from a set of vertices.
             *  @param from A set of floating-point numbers that represent the coordinates of 2D vectors,
             *  in the following format: { x1, y1, x2, y2, ... }
             *  @throws An invalid argument, if the size of the set is smaller than 6, AKA if the vertex
             *  count is smaller than 3. */
            Polygon(const std::vector<float> &from) {
                if (from.size() < 6) throw std::invalid_argument("A polygon isn't defined for vertex counts smaller than 3."); 

                this->localVertices = from;
            }


            std::vector<float> &get_vertices() {
                return localVertices;
            }

            /** @brief Sets up vertices without any scaling, rotation, or translation.
             *  @param from A set of floating-point numbers that represent the coordinates of 2D vectors,
             *  in the following format: { x1, y1, x2, y2, ... }
             *  @throws An invalid argument, if the size of the set is smaller than 6, AKA if the vertex
             *  count is smaller than 3. */
            void set_vertices(const std::vector<float> &from) {
                if (from.size() < 6) throw std::invalid_argument("Cannot set vertices if counts are smaller than 3."); 

                this->localVertices = from;
            }

            std::vector<float> &get_transformed_vertices() {
                if (!dirty) return transformedVertices;
                dirty = false;

                if (transformedVertices.size() != localVertices.size()) {
                    transformedVertices.resize(localVertices.size());
                }
                
                bool scales = scaleX != 1 || scaleY != 1;
                bool rotates = rotation != 0;

                float sine = ic::Mathf::get().sinf(rotation);
                float cosine = ic::Mathf::get().cosf(rotation);
                for (int i = 0; i < localVertices.size(); i += 2) {
                    float tx = localVertices[i], ty = localVertices[i + 1];

                    // Scaling
                    if (scales) {
                        tx *= scaleX;
                        ty *= scaleY;
                    }

                    // Rotation
                    if (rotates) {
                        float otx = tx;
                        tx = tx * cosine - ty * sine;
                        ty = otx * sine + ty * cosine;
                    }

                    // Translation
                    tx += x;
                    ty += y;

                    transformedVertices[i] = tx;
                    transformedVertices[i + 1] = ty;
                }

                return transformedVertices;
            }

            ic::Rectangle &get_bounding_box() {
                std::vector<float> verts = get_transformed_vertices();

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

                bounding.size = ic::Vec2f({maxX - minX, maxY - minY}) * 0.5f;
                bounding.position = ic::Vec2f({ minX, minY }) + bounding.size;

                return bounding;
            }

            void set_position(float px, float py) {
                x = px;
                y = py;
                dirty = true;
            }
            void set_position(ic::Vec2f &vector) {
                set_position(vector.x(), vector.y());
            }

            void set_scaling(float px, float py) {
                scaleX = px;
                scaleY = py;
                dirty = true;
            }
            void set_scaling(ic::Vec2f &vector) {
                set_scaling(vector.x(), vector.y());
            }

            void set_rotation(float radians) {
                rotation = radians;
                dirty = true;
            }



            void translate(float px, float py) {
                x += px;
                y += py;
                dirty = true;
            }
            void translate(ic::Vec2f &vector) {
                translate(vector.x(), vector.y());
            }

            void scale(float px, float py) {
                scaleX += px;
                scaleY += py;
                dirty = true;
            }
            void scale(ic::Vec2f &vector) {
                scale(vector.x(), vector.y());
            }

            void rotate(float radians) {
                rotation += radians;
                dirty = true;
            }


            float get_x() {
                return x;
            }
            float get_y() {
                return y;
            }
            float get_scaleX() {
                return scaleX;
            }
            float get_scaleY() {
                return scaleY;
            }
            float get_rotation() {
                return rotation;
            }


            bool is_dirty() {
                return dirty;
            }
            std::size_t size() {
                return localVertices.size();
            }

        protected:
            std::vector<float> localVertices;
            std::vector<float> transformedVertices;

            ic::Rectangle bounding;

            float rotation = 0.0f;
            float x = 0.0f, y = 0.0f;
            float scaleX = 1.0f, scaleY = 1.0f;

            bool dirty = false;
    };
}
#endif