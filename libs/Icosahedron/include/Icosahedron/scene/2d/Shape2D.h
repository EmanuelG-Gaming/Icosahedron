#ifndef IC_SHAPE_2D_H
#define IC_SHAPE_2D_H

#include <vector>

#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/graphics/gl/Batch.h>
#include <Icosahedron/Renderer.h>

namespace ic {
    class Shape2D {
        public:
            virtual void draw(ic::Renderer render, ic::Batch *batch, const ic::Color &color) {}

            // For shape tesselation:

            virtual std::vector<float> get_vertices() { return {}; }
            virtual std::vector<unsigned int> get_indices() { return {}; }
    };
}

#endif