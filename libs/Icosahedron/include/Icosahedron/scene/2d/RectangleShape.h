#ifndef IC_RECTANGLE_SHAPE_H
#define IC_RECTANGLE_SHAPE_H

#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/scene/2d/Shape2D.h>

namespace ic {
    class RectangleShape : public ic::Shape2D {
        public:
            ic::Vec2f position;
            ic::Vec2f size;
            
            RectangleShape();
            RectangleShape(ic::Vec2f position, ic::Vec2f size);
            
            void draw(ic::Batch2D *batch, const ic::Color &color) override;
    };
}
#endif