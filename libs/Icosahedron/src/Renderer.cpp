#include <Icosahedron/Renderer.h>

using namespace ic;

void Renderer::draw_rectangle(ic::Batch &batch, float x, float y, float width, float height, const ic::Color &color) {
    float x1 = x - width, y1 = y - height;
    float x2 = x + width, y2 = y + height;
    
    //ic::Color c = ic::Color(color).interpolate(tintColor, tinting);

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
    

    ic::Color c = ic::Color(color).interpolate(tintColor, tinting);

    std::vector<BatchVertex> vertices = {
        BatchVertex(x1, y1, entry.u, entry.v2, c),
        BatchVertex(x2, y2, entry.u2, entry.v, c),
        BatchVertex(x2, y1, entry.u2, entry.v2, c),
                   
        BatchVertex(x1, y1, entry.u, entry.v2, c),
        BatchVertex(x2, y2, entry.u2, entry.v, c),
        BatchVertex(x1, y2, entry.u, entry.v, c)
    };

    batch.add(vertices);
}

void Renderer::draw_line(ic::Batch &batch, float x1, float y1, float x2, float y2, const ic::Color &color) {
    ic::Color c = ic::Color(color).interpolate(tintColor, tinting);

    std::vector<BatchVertex> vertices = {
        BatchVertex(x1, y1, 0.0f, 0.0f, c),
        BatchVertex(x2, y2, 0.0f, 0.0f, c)
    };

    batch.add(vertices);
}
void Renderer::draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, const ic::Color &color) {
    std::vector<BatchVertex> v;

    ic::Color c = ic::Color(color).interpolate(tintColor, tinting);

    for (int index = 0; index < vertices.size(); index++) {
        ic::Vec2f vertex = vertices[index];
        v.push_back(BatchVertex(vertex.x(), vertex.y(), 0.0f, 0.0f, c));
    }

    batch.add(v);
}
void Renderer::draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<unsigned int> indices, const ic::Color &color) {
    std::vector<BatchVertex> v;

    ic::Color c = ic::Color(color).interpolate(tintColor, tinting);

    for (auto &index : indices) {
        ic::Vec2f vertex = vertices.at(index);
        v.push_back(BatchVertex(vertex.x(), vertex.y(), 0.0f, 0.0f, c));
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

void ic::Renderer::draw_string_monospace(ic::Batch &batch, ic::TextAtlas &textAtlas, const std::string &text, float x, float y, float h, float advance, const ic::Color &color) {
    std::vector<BatchVertex> vertices;
    
    float currentX = x;
    float currentY = y;
    
    float scale = h / (float) textAtlas.fontHeight;
    int glyphIndex = 0;

    std::string::const_iterator iterator;
    for (iterator = text.begin(); iterator != text.end(); iterator++) {
        char letter = (char) *iterator;

        // Break line
        if (letter == '\n') {
            currentX = x;
            currentY -= h;
        } else {
            CharacterInfo ch = textAtlas.glyph_at(*iterator);
                       
            float x1 = currentX + ch.p0x * scale;
            float y1 = currentY + ch.p0y * scale;
            float x2 = currentX + ch.p1x * scale;
            float y2 = currentY + ch.p1y * scale;
            
                            
            vertices.push_back(BatchVertex(x1, y1, ch.u0, ch.v0, color));
            vertices.push_back(BatchVertex(x2, y1, ch.u1, ch.v0, color));
            vertices.push_back(BatchVertex(x2, y2, ch.u1, ch.v1, color));
            
            vertices.push_back(BatchVertex(x1, y1, ch.u0, ch.v0, color));
            vertices.push_back(BatchVertex(x2, y2, ch.u1, ch.v1, color));
            vertices.push_back(BatchVertex(x1, y2, ch.u0, ch.v1, color));
    
            currentX += advance;
        }
        glyphIndex++;
    }
              
    batch.add(vertices);
}

void ic::Renderer::draw_string(ic::Batch &batch, ic::TextAtlas &textAtlas, const std::string &text, float x, float y, float h, const ic::Color &color) {
    std::vector<BatchVertex> vertices;
    
    float currentX = x;
    float currentY = y;
    
    float scale = h / (float) textAtlas.fontHeight;
    int glyphIndex = 0;

    std::string::const_iterator iterator;
    for (iterator = text.begin(); iterator != text.end(); iterator++) {
        char letter = (char) *iterator;

        // Break line
        if (letter == '\n') {
            currentX = x;
            currentY -= h;
        } else {
            CharacterInfo ch = textAtlas.glyph_at(*iterator);

            float x1 = currentX + ch.p0x * scale;
            float y1 = currentY + ch.p0y * scale;
            float x2 = currentX + ch.p1x * scale;
            float y2 = currentY + ch.p1y * scale;
            
                            
            vertices.push_back(BatchVertex(x1, y1, ch.u0, ch.v0, color));
            vertices.push_back(BatchVertex(x2, y1, ch.u1, ch.v0, color));
            vertices.push_back(BatchVertex(x2, y2, ch.u1, ch.v1, color));
            
            vertices.push_back(BatchVertex(x1, y1, ch.u0, ch.v0, color));
            vertices.push_back(BatchVertex(x2, y2, ch.u1, ch.v1, color));
            vertices.push_back(BatchVertex(x1, y2, ch.u0, ch.v1, color));
    
            currentX += ch.advance * scale;
        }
        glyphIndex++;
    }
              
    batch.add(vertices);
}

float Renderer::calculate_width(ic::TextAtlas &atlas, const std::string &text, float h) {
    
    float result = 0.0f;
    float scale = h / (float) atlas.fontHeight;

    std::string::const_iterator iterator;
    for (iterator = text.begin(); iterator != text.end(); iterator++) {
        CharacterInfo ch = atlas.glyph_at(*iterator); 
        result += ch.advance * scale;
    }
              
    return result;
}
        

void Renderer::tint(const ic::Color &to, float amount) {
    tintColor = to;
    tinting = amount;
}
