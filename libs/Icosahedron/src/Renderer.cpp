#include <Icosahedron/Renderer.h>

using namespace ic;

Renderer::Renderer() {
    
}

void Renderer::load() {
    fillBatch = new ic::Batch2D(10000, ic::TRIANGLES);
    lineBatch = new ic::Batch2D(10000, ic::LINES);
}

void Renderer::render() {
    fillBatch->render();
    lineBatch->render();
}

void Renderer::dispose() {
    fillBatch->dispose();
    lineBatch->dispose();
}


void Renderer::draw_rectangle(float x, float y, float width, float height, const ic::Color &color) {
    float x1 = x - width, y1 = y - height;
    float x2 = x + width, y2 = y + height;
    
    std::vector<BatchVertex> vertices = {
        BatchVertex(x1, y1, 0.0f, 1.0f, color),
        BatchVertex(x2, y2, 1.0f, 0.0f, color),
        BatchVertex(x2, y1, 1.0f, 1.0f, color),
                   
        BatchVertex(x1, y1, 0.0f, 1.0f, color),
        BatchVertex(x2, y2, 1.0f, 0.0f, color),
        BatchVertex(x1, y2, 0.0f, 0.0f, color)
    };

    fillBatch->add(vertices);
}

void Renderer::draw_Line(float x1, float y1, float x2, float y2, const ic::Color &color) {
    std::vector<BatchVertex> vertices = {
        BatchVertex(x1, y1, 0.0f, 0.0f, color),
        BatchVertex(x2, y2, 0.0f, 0.0f, color)
    };

    lineBatch->add(vertices);
}
