#ifndef IC_RECTANGLE_SHAPE_H
#define IC_RECTANGLE_SHAPE_H

#include <Icosahedron/math/geom/Vector.h>

#include <Icosahedron/scene/2d/Shape2D.h>

namespace ic {
    class RectangleShape : public ic::Shape2D {
        public:
            ic::Vector<float, 2> position;
            ic::Vector<float, 2> size;
            
            RectangleShape();
            RectangleShape(ic::Vector<float, 2> position, ic::Vector<float, 2> size);
            
            void draw(ic::Batch2D *batch, const ic::Color &color) override;
    };
}
#endif