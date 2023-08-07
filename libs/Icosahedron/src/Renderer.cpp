#include <Icosahedron/Renderer.h>

using namespace ic;

void Renderer::draw_rectangle(ic::Batch2D *batch, float x, float y, float width, float height, const ic::Color &color) {
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

    batch->add(vertices);
}

void Renderer::draw_rectangle(ic::Batch2D *batch, AtlasEntry entry, float x, float y, float width, float height, const ic::Color &color) {
    float x1 = x - width, y1 = y - height;
    float x2 = x + width, y2 = y + height;
    
    std::vector<BatchVertex> vertices = {
        BatchVertex(x1, y1, entry.u, entry.v2, color),
        BatchVertex(x2, y2, entry.u2, entry.v, color),
        BatchVertex(x2, y1, entry.u2, entry.v2, color),
                   
        BatchVertex(x1, y1, entry.u, entry.v2, color),
        BatchVertex(x2, y2, entry.u2, entry.v, color),
        BatchVertex(x1, y2, entry.u, entry.v, color)
    };

    batch->add(vertices);
}

void Renderer::draw_Line(ic::Batch2D *batch, float x1, float y1, float x2, float y2, const ic::Color &color) {
    std::vector<BatchVertex> vertices = {
        BatchVertex(x1, y1, 0.0f, 0.0f, color),
        BatchVertex(x2, y2, 0.0f, 0.0f, color)
    };

    batch->add(vertices);
}
