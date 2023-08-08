#ifndef IC_RENDERER_H
#define IC_RENDERER_H

#include <string>

#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Batch.h>
#include <Icosahedron/graphics/gl/TextureAtlas.h>

#include <Icosahedron/graphics/TextAtlas.h>
#include <Icosahedron/graphics/FreeType.h>


namespace ic {
    class Renderer {
        public:
            void draw_rectangle(ic::Batch2D *batch, float x, float y, float width, float height, const ic::Color &color = { 255, 255, 255 });
            void draw_rectangle(ic::Batch2D *batch, AtlasEntry entry, float x, float y, float width, float height, const ic::Color &color = { 255, 255, 255 });
            void draw_Line(ic::Batch2D *batch, float x1, float y1, float x2, float y2, const ic::Color &color = { 255, 255, 255 });

            void draw_string(ic::Batch2D *batch, ic::TextAtlas *textAtlas, const std::string &text, float x, float y, float scaleX = 1.0f, float scaleY = 1.0f, const ic::Color &color = { 255, 255, 255 });
            void draw_string_centered(ic::Batch2D *batch, ic::TextAtlas *textAtlas, const std::string &text, float x, float y, float scaleX = 1.0f, float scaleY = 1.0f, const ic::Color &color = { 255, 255, 255 });

        protected:
            void draw_string_itself(ic::Batch2D *batch, ic::TextAtlas *textAtlas, const std::string &text, float x, float y, float sclX, float sclY, const ic::Color &color = { 255, 255, 255 });
        
    };
}
#endif