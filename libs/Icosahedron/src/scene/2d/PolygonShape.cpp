#include <Icosahedron/scene/2d/PolygonShape.h>

using namespace ic;

PolygonShape::PolygonShape() {
    this->poly = ic::Polygon();
    this->poly.set_position(0, 0);
}
PolygonShape::PolygonShape(const std::vector<ic::Vec2f> &from, ic::Vec2f position) {
    //this->poly = ic::Polygon(from);
    //this->poly.set_position(position);

    //this->setup_polygon();
}
PolygonShape::PolygonShape(const ic::Polygon &fromPolygon, ic::Vec2f position) {
    this->poly = ic::Polygon(fromPolygon);
    this->poly.set_position(position);

    this->setup_polygon();
}

PolygonShape::PolygonShape(const std::vector<ic::Vec2f> &from, const std::vector<ic::Color> &colors, ic::Vec2f position) {
    //this->poly = ic::Polygon(from);
    //this->poly.set_position(position);
    //this->vertexColors = colors;
    
    //this->setup_polygon();
}
PolygonShape::PolygonShape(const ic::Polygon &fromPolygon, const std::vector<ic::Color> &colors, ic::Vec2f position) {
    this->poly = ic::Polygon(fromPolygon);
    this->poly.set_position(position);
    this->vertexColors = colors;

    this->setup_polygon();
}


void ic::PolygonShape::draw(ic::Renderer render, ic::Batch *batch, const ic::Color &color) {
    std::vector<float> t = this->poly.get_transformed_vertices();

    if (!vertexColors.empty()) {
        //render.draw_vertices(batch, t, indices, vertexColors, vertexTextureCoords, color);
    } else {
        //render.draw_vertices(batch, t, indices, vertexTextureCoords, color);
    }
}


void ic::PolygonShape::setup_polygon() {
    /*
    std::vector<float> vertices = this->poly.get_vertices();
    ic::Rectangle bounds = this->poly.get_bounding_box();

    this->indices = ic::EarClippingTriangulation::get().triangulate(vertices);

    for (int i = 0; i < vertices.size(); i += 2) {
        float x = vertices[i];
        float y = vertices[i + 1];

        ic::Vec2f normalized = (vertex / bounds.size + 1.0f) * 0.5f;

        this->vertexTextureCoords.push_back(normalized);
    }
    */
}