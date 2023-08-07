#ifndef IC_RECTANGLE_SHAPE_H
#define IC_RECTANGLE_SHAPE_H

#include <string>

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/geom/Rectangle.h>

#include <Icosahedron/scene/2d/Shape2D.h>
#include <Icosahedron/Renderer.h>


namespace ic {
    class RectangleShape : public ic::Shape2D {
        public:
            ic::Rectangle r;

            RectangleShape();
            RectangleShape(ic::Vec2f position, ic::Vec2f size, std::string textureName = "");
            
            void draw(ic::Renderer render, ic::Batch2D *batch, const ic::Color &color) override;

            void set_atlas(ic::TextureAtlas *to);
            
        private:
            ic::TextureAtlas *atlas = nullptr;
            std::string textureName;
    };
}
#endif