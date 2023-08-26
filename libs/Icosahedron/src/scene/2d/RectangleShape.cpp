#include <Icosahedron/scene/2d/RectangleShape.h>

#include <vector>

using namespace ic;

RectangleShape::RectangleShape() {
    r.position = { 0.0f, 0.0f };
    r.size = { 0.2f, 0.2f };
}

RectangleShape::RectangleShape(ic::Vec2f position, ic::Vec2f size, std::string textureName) {
    this->r.position = position;
    this->r.size = size;
    this->textureName = textureName;
}

void ic::RectangleShape::draw(ic::Renderer render, ic::Batch *batch, const ic::Color &color) {
    if (atlas != nullptr) {
        if (textureName.empty()) return;
        
        render.draw_rectangle(batch, atlas->get_entry(textureName),
                            r.position.x(), r.position.y(),
                            r.size.x(), r.size.y(),
                            color);
    } else {
        render.draw_rectangle(batch,
                            r.position.x(), r.position.y(),
                            r.size.x(), r.size.y(),
                            color);
    }
}

void ic::RectangleShape::set_atlas(ic::TextureAtlas *to) {
    this->atlas = to;
}
            