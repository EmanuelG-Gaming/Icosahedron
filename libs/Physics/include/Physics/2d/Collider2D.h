#ifndef IC_PHYSICS_COLLIDER_2D_H
#define IC_PHYSICS_COLLIDER_2D_H

#include <string>

#include <Icosahedron/math/geom/Polygon.h>
#include <Icosahedron/math/geom/Rectangle.h>

#include <Physics/2d/ManifoldPoints2D.h>
#include <Physics/2d/Transform2D.h>


namespace ic { namespace Physics {
    struct Collider2D;
    struct CircleCollider;
    struct RectangleCollider;
    //struct SpringMassCollider2D;
    struct PolygonCollider;

    struct Collider2D {
        Collider2D() {}

        virtual ManifoldPoints2D test(Transform2D *TransformA, Collider2D *colliderB, Transform2D *transformB)           = 0;
        virtual ManifoldPoints2D test(Transform2D *TransformA, CircleCollider *colliderB, Transform2D *TransformB)       = 0;
        virtual ManifoldPoints2D test(Transform2D *TransformA, RectangleCollider *colliderB, Transform2D *TransformB)    = 0;
        //virtual ManifoldPoints2D test(Transform2D *TransformA, SpringMassCollider2D *colliderB, Transform2D *TransformB) = 0;
        virtual ManifoldPoints2D test(Transform2D *TransformA, PolygonCollider *colliderB, Transform2D *TransformB)    = 0;
    };

    struct CircleCollider : Collider2D {
        public:
            float radius = 1.0f;

            CircleCollider() {}
            CircleCollider(float radius) : radius(radius) {}

            ManifoldPoints2D test(Transform2D *TransformA, Collider2D *colliderB, Transform2D *transformB)           override;
            ManifoldPoints2D test(Transform2D *TransformA, CircleCollider *colliderB, Transform2D *TransformB)       override;
            ManifoldPoints2D test(Transform2D *TransformA, RectangleCollider *colliderB, Transform2D *TransformB)    override;
            //ManifoldPoints2D test(Transform2D *TransformA, SpringMassCollider2D *colliderB, Transform2D *TransformB) override;
            ManifoldPoints2D test(Transform2D *TransformA, PolygonCollider *colliderB, Transform2D *TransformB)    override;
    };

    struct RectangleCollider : Collider2D {
        public:
            float width = 1.0f, height = 1.0f;
            
            RectangleCollider() {}
            RectangleCollider(float width, float height) : width(width), height(height) {}

            ManifoldPoints2D test(Transform2D *TransformA, Collider2D *colliderB, Transform2D *transformB)           override;
            ManifoldPoints2D test(Transform2D *TransformA, CircleCollider *colliderB, Transform2D *TransformB)       override;
            ManifoldPoints2D test(Transform2D *TransformA, RectangleCollider *colliderB, Transform2D *TransformB)    override;
            //ManifoldPoints2D test(Transform2D *TransformA, SpringMassCollider2D *colliderB, Transform2D *TransformB) override;
            ManifoldPoints2D test(Transform2D *TransformA, PolygonCollider *colliderB, Transform2D *TransformB)    override;
    };

    struct PolygonCollider : Collider2D {
        public:
            std::vector<ic::Vec2f> modelVertices;
            ic::Rectangle bounding;

            PolygonCollider() {}
            PolygonCollider(const std::vector<ic::Vec2f> &modelVertices) : modelVertices(modelVertices) {
                this->bounding = this->calculate_bounding_box();
            }
            PolygonCollider(const std::vector<float> &vertices) {
                std::vector<ic::Vec2f> to;

                for (int i = 0; i < vertices.size(); i += 2) {
                    to.push_back({ vertices[i], vertices[i + 1] });
                }

                this->modelVertices = to;
                this->bounding = this->calculate_bounding_box();
            }

            ic::Rectangle calculate_bounding_box() {
                float minX = modelVertices[0].x();
                float minY = modelVertices[0].y();
                float maxX = modelVertices[0].x();
                float maxY = modelVertices[0].y();

                for (int i = 0; i < modelVertices.size(); i += 2) {
                    float dx = modelVertices[i].x();
                    float dy = modelVertices[i].y();

                    minX = minX > dx ? dx : minX;
                    minY = minY > dy ? dy : minY;

                    maxX = maxX < dx ? dx : maxX;
                    maxY = maxY < dy ? dy : maxY;
                }

                ic::Rectangle result;
                result.size = ic::Vec2f({maxX - minX, maxY - minY}) * 0.5f;
                result.position = ic::Vec2f({ minX, minY }) + result.size;

                return result;
            }


            ManifoldPoints2D test(Transform2D *TransformA, Collider2D *colliderB, Transform2D *transformB)           override;
            ManifoldPoints2D test(Transform2D *TransformA, CircleCollider *colliderB, Transform2D *TransformB)       override;
            ManifoldPoints2D test(Transform2D *TransformA, RectangleCollider *colliderB, Transform2D *TransformB)    override;
            //ManifoldPoints2D test(Transform2D *TransformA, SpringMassCollider2D *colliderB, Transform2D *TransformB) override;
            ManifoldPoints2D test(Transform2D *TransformA, PolygonCollider *colliderB, Transform2D *TransformB)    override;
    };

    //struct SpringMassCollider2D : Collider2D {
    //    public:
    //        SpringMassCollider2D() {}
    //        
    //        ManifoldPoints2D test(Transform2D *TransformA, Collider2D *colliderB, Transform2D *transformB)           override;
    //        ManifoldPoints2D test(Transform2D *TransformA, CircleCollider *colliderB, Transform2D *TransformB)       override;
    //        ManifoldPoints2D test(Transform2D *TransformA, RectangleCollider *colliderB, Transform2D *TransformB)    override;
    //        ManifoldPoints2D test(Transform2D *TransformA, SpringMassCollider2D *colliderB, Transform2D *TransformB) override;
    //};
}}

#endif