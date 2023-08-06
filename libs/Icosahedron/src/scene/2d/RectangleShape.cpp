#include <Icosahedron/scene/2d/RectangleShape.h>

#include <vector>

using namespace ic;

RectangleShape::RectangleShape() {
    r.position = { 0.0f, 0.0f };
    r.size = { 0.2f, 0.2f };
}

RectangleShape::RectangleShape(ic::Vec2f position, ic::Vec2f size) {
    this->r.position = position;
    this->r.size = size;
}

void RectangleShape::draw(ic::Batch2D *batch, const ic::Color &color) {
    float x1 = r.position.x() - r.size.x(), y1 = r.position.y() - r.size.y();
    float x2 = r.position.x() + r.size.x(), y2 = r.position.y() + r.size.y();
    
    std::vector<BatchVertex> vertices = {
        BatchVertex(x1, y1, 0.0f, 1.0f, color),
        BatchVertex(x2, y1, 1.0f, 1.0f, color),
        BatchVertex(x2, y2, 1.0f, 0.0f, color),
                   
        BatchVertex(x1, y1, 0.0f, 1.0f, color),
        BatchVertex(x2, y2, 1.0f, 0.0f, color),
        BatchVertex(x1, y2, 0.0f, 0.0f, color)
    };

    batch->add(vertices);
}
            