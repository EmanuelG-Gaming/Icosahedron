#include <Icosahedron/Renderer.h>

using namespace ic;

void Renderer::draw_rectangle(ic::Batch &batch, float x, float y, float width, float height, const ic::Color &color) {
    float x1 = x - width, y1 = y - height;
    float x2 = x + width, y2 = y + height;
    
    std::vector<BatchVertex> vertices = {
        BatchVertex(x1, y1, 0.0f, 0.0f, color),
        BatchVertex(x2, y2, 1.0f, 1.0f, color),
        BatchVertex(x2, y1, 1.0f, 0.0f, color),
                   
        BatchVertex(x1, y1, 0.0f, 0.0f, color),
        BatchVertex(x2, y2, 1.0f, 1.0f, color),
        BatchVertex(x1, y2, 0.0f, 1.0f, color)
    };

    batch.add(vertices);
}

void Renderer::draw_rectangle(ic::Batch &batch, AtlasEntry entry, float x, float y, float width, float height, const ic::Color &color) {
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

    batch.add(vertices);
}

void Renderer::draw_line(ic::Batch &batch, float x1, float y1, float x2, float y2, const ic::Color &color) {
    std::vector<BatchVertex> vertices = {
        BatchVertex(x1, y1, 0.0f, 0.0f, color),
        BatchVertex(x2, y2, 0.0f, 0.0f, color)
    };

    batch.add(vertices);
}
void Renderer::draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, const ic::Color &color) {
    std::vector<BatchVertex> v;

    for (int index = 0; index < vertices.size(); index++) {
        ic::Vec2f vertex = vertices[index];
        v.push_back(BatchVertex(vertex.x(), vertex.y(), 0.0f, 0.0f, color));
    }

    batch.add(v);
}
void Renderer::draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<unsigned int> indices, const ic::Color &color) {
    std::vector<BatchVertex> v;

    for (auto &index : indices) {
        ic::Vec2f vertex = vertices.at(index);
        v.push_back(BatchVertex(vertex.x(), vertex.y(), 0.0f, 0.0f, color));
    }

    batch.add(v);
}

void Renderer::draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<ic::Color> vertexColors, const ic::Color &color) {
    if (vertexColors.size() != vertices.size()) {
        printf("Didn't render the vertices. Vertex colors' size didn't correspond to the vertex positions' size.\n");
        return;
    }

    std::vector<BatchVertex> v;
    for (int index = 0; index < vertices.size(); index++) {
        ic::Vec2f vertex = vertices[index];
        ic::Color col = vertexColors[index], tint = color;
        ic::Color c = col.interpolate(tint, tinting);

        v.push_back(BatchVertex(vertex.x(), vertex.y(), 0.0f, 0.0f, c));
    }

    batch.add(v);
}
void Renderer::draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<unsigned int> indices, std::vector<ic::Color> vertexColors, const ic::Color &color) {
    if (vertexColors.size() != vertices.size()) {
        printf("Didn't render the vertices. Vertex colors' size didn't correspond to the vertex positions' size.\n");
        return;
    }

    std::vector<BatchVertex> v;

    for (auto &index : indices) {
        ic::Vec2f vertex = vertices.at(index);
        ic::Color col = vertexColors[index], tint = color;
        ic::Color c = col.interpolate(tint, tinting);

        v.push_back(BatchVertex(vertex.x(), vertex.y(), 0.0f, 0.0f, c));
    }

    batch.add(v);
}

void Renderer::draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<ic::Color> vertexColors, std::vector<ic::Vec2f> vertexTexCoords, const ic::Color &color) {
    if (vertexColors.size() != vertices.size()) {
        printf("Didn't render the vertices. Vertex colors' size didn't correspond to the vertex positions' size.\n");
        return;
    }
    if (vertexTexCoords.size() != vertices.size()) {
        printf("Didn't render the vertices. Vertex texture coordinates' size didn't correspond to the vertex positions' size.\n");
        return;
    }

    std::vector<BatchVertex> v;

    for (int index = 0; index < vertices.size(); index++) {
        ic::Vec2f vertex = vertices.at(index);
        ic::Color col = vertexColors[index], tint = color;
        ic::Color c = col.interpolate(tint, tinting);
        ic::Vec2f coords = vertexTexCoords[index];

        v.push_back(BatchVertex(vertex.x(), vertex.y(), coords.x(), coords.y(), c));
    }

    batch.add(v);
}

void Renderer::draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<unsigned int> indices, std::vector<ic::Color> vertexColors, std::vector<ic::Vec2f> vertexTexCoords, const ic::Color &color) {
    if (vertexColors.size() != vertices.size()) {
        printf("Didn't render the vertices. Vertex colors' size didn't correspond to the vertex positions' size.\n");
        return;
    }
    if (vertexTexCoords.size() != vertices.size()) {
        printf("Didn't render the vertices. Vertex texture coordinates' size didn't correspond to the vertex positions' size.\n");
        return;
    }

    std::vector<BatchVertex> v;

    for (auto &index : indices) {
        ic::Vec2f vertex = vertices.at(index);
        ic::Color col = vertexColors[index], tint = color;
        ic::Color c = col.interpolate(tint, tinting);
        ic::Vec2f coords = vertexTexCoords[index];

        v.push_back(BatchVertex(vertex.x(), vertex.y(), coords.x(), coords.y(), c));
    }

    batch.add(v);
}



void Renderer::draw_string(ic::Batch &batch, ic::TextAtlas &textAtlas, const std::string &text, float x, float y, float scaleX, float scaleY, const ic::Color &color) {
    float sclX = scaleX * 0.002f;
    float sclY = scaleY * 0.002f;
           
    draw_string_itself(batch, textAtlas, text, x, y, sclX, sclY, color);
}
void Renderer::draw_string_centered(ic::Batch &batch, ic::TextAtlas &textAtlas, const std::string &text, float x, float y, float scaleX, float scaleY, const ic::Color &color) {
    float sclX = scaleX * 0.002f;
    float sclY = scaleY * 0.002f;
        
    float w = 0.0f;
    float h = 0.0f;
    std::string::const_iterator iterator;
    for (iterator = text.begin(); iterator != text.end(); iterator++) {
        CharacterInfo ch = textAtlas.get_characters().at(*iterator);
                
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


void Renderer::draw_string_itself(ic::Batch &batch, ic::TextAtlas &textAtlas, const std::string &text, float x, float y, float sclX, float sclY, const ic::Color &color) {
    std::vector<BatchVertex> vertices;
    
    float px = x;
    float py = y;
    std::string::const_iterator iterator;
    for (iterator = text.begin(); iterator != text.end(); iterator++) {
        CharacterInfo ch = textAtlas.get_characters().at(*iterator);
                   
        float x2 = px + ch.bitmapLeft * sclX;
        float y2 = -py - ch.bitmapTop * sclY;
        float width = ch.bitmapWidth * sclX;
        float height = ch.bitmapHeight * sclY;
                   
        px += ch.advX * sclX;
        py += ch.advY * sclY;
                   
                   
        vertices.push_back(BatchVertex(x2, -y2, ch.offsetX, 0, color));
        vertices.push_back(BatchVertex(x2 + width, -y2, ch.offsetX + ch.bitmapWidth / textAtlas.get_width(), 0, color));
        vertices.push_back(BatchVertex(x2, -y2 - height, ch.offsetX, ch.bitmapHeight / textAtlas.get_height(), color));
                   
        vertices.push_back(BatchVertex(x2 + width, -y2, ch.offsetX + ch.bitmapWidth / textAtlas.get_width(), 0, color));
        vertices.push_back(BatchVertex(x2 + width, -y2 - height, ch.offsetX + ch.bitmapWidth / textAtlas.get_width(), ch.bitmapHeight / textAtlas.get_height(), color));
        vertices.push_back(BatchVertex(x2, -y2 - height, ch.offsetX, ch.bitmapHeight / textAtlas.get_height(), color));
    }
              
    batch.add(vertices);
}
        
void Renderer::tint(float amount) {
    tinting = amount;
}