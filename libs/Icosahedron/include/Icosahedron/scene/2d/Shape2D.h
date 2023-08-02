#ifndef IC_SHAPE_2D_H
#define IC_SHAPE_2D_H

#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/graphics/gl/Batch.h>

namespace ic {
    class Shape2D {
        public:
            virtual void draw(ic::Batch2D *batch, const ic::Color &color) {}
    };
}

#endif