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
void Renderer::draw_vertices(ic::Batch2D *batch, std::vector<ic::Vec2f> vertices, std::vector<int> indices, const ic::Color &color) {
    std::vector<BatchVertex> v;

    for (auto &index : indices) {
        ic::Vec2f vertex = vertices[index];
        v.push_back(BatchVertex(vertex.x(), vertex.y(), 0.0f, 0.0f, color));
    }

    batch->add(v);
}

void Renderer::draw_string(ic::Batch2D *batch, ic::TextAtlas *textAtlas, const std::string &text, float x, float y, float scaleX, float scaleY, const ic::Color &color) {
    float sclX = scaleX * 0.002f;
    float sclY = scaleY * 0.002f;
           
    draw_string_itself(batch, textAtlas, text, x, y, sclX, sclY, color);
}
void Renderer::draw_string_centered(ic::Batch2D *batch, ic::TextAtlas *textAtlas, const std::string &text, float x, float y, float scaleX, float scaleY, const ic::Color &color) {
    float sclX = scaleX * 0.002f;
    float sclY = scaleY * 0.002f;
        
    float w = 0.0f;
    float h = 0.0f;
    std::string::const_iterator iterator;
    for (iterator = text.begin(); iterator != text.end(); iterator++) {
        CharacterInfo ch = textAtlas->get_characters().at(*iterator);
                
        w += ch.advX;
                
        if (ch.bitmapHeight > h) {
            h = ch.bitmapHeight;
        }
    }
    
           
    w *= sclX;
    h *= sclY;
           
           
    float px = x - w / 2.0f;
    float py = y - h / 2.0f;
    
    draw_string_itself(batch, textAtlas, text, px, py, sclX, sclY, color); 
}


void Renderer::draw_string_itself(ic::Batch2D *batch, ic::TextAtlas *textAtlas, const std::string &text, float x, float y, float sclX, float sclY, const ic::Color &color) {
    std::vector<BatchVertex> vertices;
    
    float px = x;
    float py = y;
    std::string::const_iterator iterator;
    for (iterator = text.begin(); iterator != text.end(); iterator++) {
        CharacterInfo ch = textAtlas->get_characters().at(*iterator);
                   
        float x2 = px + ch.bitmapLeft * sclX;
        float y2 = -py - ch.bitmapTop * sclY;
        float width = ch.bitmapWidth * sclX;
        float height = ch.bitmapHeight * sclY;
                   
        px += ch.advX * sclX;
        py += ch.advY * sclY;
                   
                   
        vertices.push_back(BatchVertex(x2, -y2, ch.offsetX, 0, color));
        vertices.push_back(BatchVertex(x2 + width, -y2, ch.offsetX + ch.bitmapWidth / textAtlas->get_width(), 0, color));
        vertices.push_back(BatchVertex(x2, -y2 - height, ch.offsetX, ch.bitmapHeight / textAtlas->get_height(), color));
                   
        vertices.push_back(BatchVertex(x2 + width, -y2, ch.offsetX + ch.bitmapWidth / textAtlas->get_width(), 0, color));
        vertices.push_back(BatchVertex(x2 + width, -y2 - height, ch.offsetX + ch.bitmapWidth / textAtlas->get_width(), ch.bitmapHeight / textAtlas->get_height(), color));
        vertices.push_back(BatchVertex(x2, -y2 - height, ch.offsetX, ch.bitmapHeight / textAtlas->get_height(), color));
    }
              
    batch->add(vertices);
}
        