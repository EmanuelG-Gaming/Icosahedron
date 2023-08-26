#ifndef IC_POLYGON_SHAPE_H
#define IC_POLYGON_SHAPE_H

#include <vector>

#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/geom/Rectangle.h>
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
            PolygonShape(const std::vector<ic::Vec2f> &from, const std::vector<ic::Color> &colors, ic::Vec2f position = {0, 0});
            PolygonShape(const ic::Polygon &fromPolygon, const std::vector<ic::Color> &colors, ic::Vec2f position = {0, 0});

            void draw(ic::Renderer render, ic::Batch *batch, const ic::Color &color) override;
            
        protected:
            void setup_polygon();

        private:
            std::vector<int> indices;
            // For per-vertex coloring
            std::vector<ic::Color> vertexColors;

            std::vector<ic::Vec2f> vertexTextureCoords;
    };
}
#endif