#include <Icosahedron/scene/2d/PolygonShape.h>

using namespace ic;

PolygonShape::PolygonShape() {
    this->poly = ic::Polygon();
    this->poly.set_position(0, 0);
}
PolygonShape::PolygonShape(const std::vector<ic::Vec2f> &from, ic::Vec2f position) {
    this->poly = ic::Polygon(from);
    this->poly.set_position(position);
}
PolygonShape::PolygonShape(const ic::Polygon &fromPolygon, ic::Vec2f position) {
    this->poly = ic::Polygon(fromPolygon);
    this->poly.set_position(position);
}

void ic::PolygonShape::draw(ic::Renderer render, ic::Batch2D *batch, const ic::Color &color) {
    std::vector<ic::Vec2f> vertices = this->poly.get_vertices();
    std::vector<int> indices = ic::EarClippingTriangulation::get().triangulate(vertices);

    render.draw_vertices(batch, vertices, indices, color);
}