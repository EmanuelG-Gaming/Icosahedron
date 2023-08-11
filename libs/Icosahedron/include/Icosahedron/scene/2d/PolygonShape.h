#ifndef IC_POLYGON_SHAPE_H
#define IC_POLYGON_SHAPE_H

#include <Icosahedron/math/geom/Polygon.h>
#include <Icosahedron/math/EarClippingTriangulation.h>

#include <Icosahedron/scene/2d/Shape2D.h>

namespace ic {
    class PolygonShape : public ic::Shape2D {
        public:
            ic::Polygon poly;

            PolygonShape();
            PolygonShape(const std::vector<ic::Vec2f> &from, ic::Vec2f position = {0, 0});
            PolygonShape(const ic::Polygon &fromPolygon, ic::Vec2f position = {0, 0});

            void draw(ic::Renderer render, ic::Batch2D *batch, const ic::Color &color) override;
    };
}
#endif