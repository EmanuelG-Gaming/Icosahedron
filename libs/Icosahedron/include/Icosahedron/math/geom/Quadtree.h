#ifndef IC_QUADTREE_H
#define IC_QUADTREE_H

#include <Icosahedron/math/geom/Rectangle.h>
#include <list>
#include <functional>


namespace ic {
    template <typename T>
    struct QuadtreeLocation {
        typename std::list<std::pair<ic::Rectangle, T>>* container;
        typename std::list<std::pair<ic::Rectangle, T>>::iterator iterator;
    };

    
    struct QuadtreeRootData {
        /** @brief The maximum number of layers that a quadtree instance can pass through. */
        int depth;

        /** @brief How many elements can a quadtree instance (node) support? 
         *  @note The maximum number of elements that a whole quadtree can theoretically support is the sum from k=0 to depth of 2^(k * 2)
         *  multiplied by the capacity of a single node.
         * 
        */
        std::size_t nodeCapacity;

        QuadtreeRootData(std::size_t nodeCapacity, int depth) : nodeCapacity(nodeCapacity), depth(depth) {
        }
    };


    // ----- Static quadtree -----
    template <typename T>
    struct StaticQuadtree {
        public:
            StaticQuadtree(ic::Rectangle boundary, QuadtreeRootData data) {
                layer = 0;
                dataRef = &data;

                resize(boundary);
            }

            /** @warning Do not use this directly! Use StaticQuadtree(ic::Rectangle boundary, QuadtreeRootData data)
             *  if you want to declare a quadtree root (1st layer).
             */
            StaticQuadtree(ic::Rectangle boundary, StaticQuadtree *parent) {
                layer = 0;
                dataRef = parent->dataRef;
                
                resize(boundary);
            }


            bool insert(T element, ic::Rectangle aabb) {
                if (!boundary.overlaps(aabb)) {
                    return false;
                }

                std::pair<ic::Rectangle, T> pair = std::make_pair(aabb, element);

                if (elements.size() < this->dataRef->nodeCapacity && !has_subdivisions()) {
                    elements.push_back(pair);
                    return true;
                }

                if (layer >= this->dataRef->depth) {
                    return false;
                }

                if (!has_subdivisions()) {
                    subdivide();
                }

                if (bottomLeft->insert(pair.second, pair.first)) return true;
                if (bottomRight->insert(pair.second, pair.first)) return true;
                if (topRight->insert(pair.second, pair.first)) return true;
                if (topLeft->insert(pair.second, pair.first)) return true;

                return false;
            }

            std::list<T> query_from_range(ic::Rectangle range) {
                std::list<T> result;

                if (!boundary.overlaps(range)) {
                    return result;
                }

                for (auto &element : elements) {
                    if (range.overlaps(element.first)) {
                        result.push_back(element.second);
                    }
                }

                if (!has_subdivisions()) {
                    return result;
                }

                insert_to_list(result, bottomLeft->query_from_range(range));
                insert_to_list(result, bottomRight->query_from_range(range));
                insert_to_list(result, topRight->query_from_range(range));
                insert_to_list(result, topLeft->query_from_range(range));

                return result;
            }

            void insert_to_list(std::list<T> &A, std::list<T> B) {
                for (auto &element : B) {
                    A.push_back(element);
                }
            }

            void subdivide() {
                float halfWidth = boundary.width / 2.0f;
                float halfHeight = boundary.height / 2.0f;

                bottomLeft = std::make_shared<ic::StaticQuadtree<T>>(ic::Rectangle({ boundary.position.x() - halfWidth, boundary.position.y() - halfHeight }, { halfWidth, halfHeight }), this);
                bottomRight = std::make_shared<ic::StaticQuadtree<T>>(ic::Rectangle({ boundary.position.x() + halfWidth, boundary.position.y() - halfHeight }, { halfWidth, halfHeight }), this);
                topRight = std::make_shared<ic::StaticQuadtree<T>>(ic::Rectangle({ boundary.position.x() + halfWidth, boundary.position.y() + halfHeight }, { halfWidth, halfHeight }), this);
                topLeft = std::make_shared<ic::StaticQuadtree<T>>(ic::Rectangle({ boundary.position.x() - halfWidth, boundary.position.y() + halfHeight }, { halfWidth, halfHeight }), this);
                
                bottomLeft->layer = bottomRight->layer = topRight->layer = topLeft->layer = this->layer + 1;
            }

            void loop(const std::function<void(ic::Rectangle)> &function) {
                function(this->boundary);

                if (has_subdivisions()) {
                    bottomLeft->loop(function);
                    bottomRight->loop(function);
                    topRight->loop(function);
                    topLeft->loop(function);
                }
            }

            void resize(ic::Rectangle newBoundary) {
                erase();
                boundary = newBoundary;
            }

            void erase() {
                elements.clear();
                layer = 0;

                if (has_subdivisions()) {
                    bottomLeft->erase();
                    bottomRight->erase();
                    topRight->erase();
                    topLeft->erase();
                }

                bottomLeft.reset();
                bottomRight.reset();
                topRight.reset();
                topLeft.reset();
            }

            bool has_subdivisions() {
                return (bottomLeft || bottomRight || topRight || topLeft);
            }

        
        private:
            ic::Rectangle boundary;
            std::vector<std::pair<ic::Rectangle, T>> elements;
            int layer;

            std::shared_ptr<StaticQuadtree<T>> bottomLeft, bottomRight, topRight, topLeft;

            QuadtreeRootData *dataRef;
    };
};

#endif