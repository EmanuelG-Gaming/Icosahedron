#ifndef IC_RENDERER_H
#define IC_RENDERER_H

#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Batch.h>


namespace ic {
    class Renderer {
        public:
            Renderer();

            void load();
            void render();
            void dispose();

            void draw_rectangle(float x, float y, float width, float height, const ic::Color &color = { 255, 255, 255 });
            void draw_Line(float x1, float y1, float x2, float y2, const ic::Color &color = { 255, 255, 255 });

        protected:
            ic::Batch2D *fillBatch, *lineBatch;
    };
}
#endif