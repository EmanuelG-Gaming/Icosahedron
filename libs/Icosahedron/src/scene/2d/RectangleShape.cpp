#include <Icosahedron/scene/2d/RectangleShape.h>

#include <vector>

using namespace ic;

RectangleShape::RectangleShape() {
    position = { 0.0f, 0.0f };
    size = { 0.2f, 0.2f };
}

RectangleShape::RectangleShape(ic::Vector<float, 2> position, ic::Vector<float, 2> size) {
    this->position = position;
    this->size = size;
}

void RectangleShape::draw(ic::Batch2D *batch, const ic::Color &color) {
    float x1 = position[0] - size[0], y1 = position[1] - size[1];
    float x2 = position[0] + size[0], y2 = position[1] + size[1];
    
    std::vector<BatchVertex> vertices = {
        BatchVertex(x1, y1, color),
        BatchVertex(x2, y1, color),
        BatchVertex(x2, y2, color),
                   
        BatchVertex(x1, y1, color),
        BatchVertex(x2, y2, color),
        BatchVertex(x1, y2, color)
    };

    batch->add(vertices);
}
            