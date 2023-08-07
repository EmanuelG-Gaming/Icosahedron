#ifndef IC_RENDERER_H
#define IC_RENDERER_H

#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Batch.h>
#include <Icosahedron/graphics/gl/TextureAtlas.h>

namespace ic {
    class Renderer {
        public:
            void draw_rectangle(ic::Batch2D *batch, float x, float y, float width, float height, const ic::Color &color = { 255, 255, 255 });
            void draw_rectangle(ic::Batch2D *batch, AtlasEntry entry, float x, float y, float width, float height, const ic::Color &color = { 255, 255, 255 });
            void draw_Line(ic::Batch2D *batch, float x1, float y1, float x2, float y2, const ic::Color &color = { 255, 255, 255 });
    };
}
#endif