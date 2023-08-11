#ifndef IC_POLYGON_H
#define IC_POLYGON_H

#include <vector>
#include <initializer_list>
#include <stdexcept>

#include <Icosahedron/math/Mathf.h>
#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/geom/Rectangle.h>

namespace ic {
    /* Illustrates a polygon with local and transformed vertices in two-dimensional space. */
    struct Polygon {
        public:
            /* Initializes a new, but empty polygon. */
            Polygon() {
                this->localVertices.reserve(0);
            }

            /* Constructs a polygon from a set of vertices.
             * Throws an invalid argument if the vertex count is smaller than 3. */
            Polygon(const std::vector<ic::Vec2f> &from) {
                if (from.size() < 3) throw std::invalid_argument("A polygon isn't defined for vertex counts smaller than 3."); 

                this->localVertices = from;
                /*
                for (auto position = from.begin(); position != from.end(); position++) {
                    this->localVertices[std::distance(from.begin(), position)] = *position;
                }
                */
            }


            std::vector<ic::Vec2f> &get_vertices() {
                return localVertices;
            }

            /* Sets vertices without any scaling, rotation, or translation.
             * Throws an invalid argument if the vertex count is smaller than 3. */
            void set_vertices(const std::vector<ic::Vec2f> &from) {
                if (from.size() < 3) throw std::invalid_argument("Cannot set vertices if counts are smaller than 3."); 

                this->localVertices = from;
            }

            std::vector<ic::Vec2f> &get_transformed_vertices() {
                if (!dirty) return transformedVertices;
                dirty = false;

                if (localVertices.size() != transformedVertices.size()) {
                    transformedVertices.reserve(localVertices.size());
                }

                float sine = ic::Mathf::get().sinf(rotation);
                float cosine = ic::Mathf::get().cosf(rotation);
                for (int i = 0; i < localVertices.size(); i++) {
                    ic::Vec2f local = localVertices[i];
                    float tx = local.x(), ty = local.y();

                    // Scaling
                    tx *= scaleX;
                    ty *= scaleY;

                    // Rotation
                    float otx = tx;
                    tx = tx * cosine - ty * sine;
                    ty = otx * sine + ty * cosine;

                    // Translation
                    tx += x;
                    ty += y;

                    ic::Vec2f transformed = { tx, ty };
                    transformedVertices[i] = transformed;
                }

                return transformedVertices;
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
            std::vector<ic::Vec2f> localVertices;
            std::vector<ic::Vec2f> transformedVertices;

            ic::Rectangle bounding;

            float rotation = 0;
            float x = 0, y = 0;
            float scaleX = 1, scaleY = 1;

            bool dirty = false;
    };
}
#endif