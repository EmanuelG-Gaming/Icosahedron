#include <Icosahedron/scene/2d/RectangleShape.h>

#include <vector>

using namespace ic;

RectangleShape::RectangleShape() {
    position = { 0.0f, 0.0f };
    size = { 0.2f, 0.2f };
}

RectangleShape::RectangleShape(ic::Vec2f position, ic::Vec2f size) {
    this->position = position;
    this->size = size;
}

void RectangleShape::draw(ic::Batch2D *batch, const ic::Color &color) {
    float x1 = position.x() - size.x(), y1 = position.y() - size.y();
    float x2 = position.x() + size.x(), y2 = position.y() + size.y();
    
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
            