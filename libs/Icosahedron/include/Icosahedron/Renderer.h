#ifndef IC_RENDERER_H
#define IC_RENDERER_H

#include <string>
#include <vector>

#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Batch.h>
#include <Icosahedron/graphics/gl/TextureAtlas.h>

#include <Icosahedron/graphics/TextAtlas.h>
#include <Icosahedron/graphics/FreeType.h>


namespace ic {
    class Renderer {
        public:
            void draw_rectangle(ic::Batch &batch, float x, float y, float width, float height, const ic::Color &color = { 255, 255, 255 });
            void draw_rectangle(ic::Batch &batch, AtlasEntry entry, float x, float y, float width, float height, const ic::Color &color = { 255, 255, 255 });
            void draw_line(ic::Batch &batch, float x1, float y1, float x2, float y2, const ic::Color &color = { 255, 255, 255 });
            
            void draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, const ic::Color &color = { 255, 255, 255 }); 
            void draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<unsigned int> indices, const ic::Color &color = { 255, 255, 255 }); 
            
            void draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<ic::Color> vertexColors, const ic::Color &color = { 255, 255, 255 }); 
            void draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<unsigned int> indices, std::vector<ic::Color> vertexColors, const ic::Color &color = { 255, 255, 255 }); 
            
            void draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<ic::Color> vertexColors, std::vector<ic::Vec2f> vertexTexCoords, const ic::Color &color = { 255, 255, 255 }); 
            void draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<unsigned int> indices, std::vector<ic::Color> vertexColors, std::vector<ic::Vec2f> vertexTexCoords, const ic::Color &color = { 255, 255, 255 }); 



            //void draw_string(ic::Batch &batch, ic::TextAtlas &textAtlas, const std::string &text, float x, float y, float scaleX = 1.0f, float scaleY = 1.0f, const ic::Color &color = { 255, 255, 255 });
            //void draw_string_centered(ic::Batch &batch, ic::TextAtlas &textAtlas, const std::string &text, float x, float y, float scaleX = 1.0f, float scaleY = 1.0f, const ic::Color &color = { 255, 255, 255 });

            void tint(const ic::Color &to, float amount);
            
        protected:
            //void draw_string_itself(ic::Batch &batch, ic::TextAtlas &textAtlas, const std::string &text, float x, float y, float sclX, float sclY, const ic::Color &color = { 255, 255, 255 });

        private:
           float tinting = 0.0f;
           ic::Color tintColor = { 255, 255, 255 };
    };
}
#endif