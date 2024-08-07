#ifndef IC_RENDERER_H
#define IC_RENDERER_H

#include <string>
#include <vector>

#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Batch.h>
#include <Icosahedron/graphics/gl/TextureAtlas.h>

#include <Icosahedron/graphics/TextAtlas.h>


namespace ic {
    namespace Renderer {
        void draw_rectangle(ic::Batch &batch, float x, float y, float width, float height, const ic::Color &color = { 255, 255, 255 });
        void draw_rectangle(ic::Batch &batch, AtlasEntry entry, float x, float y, float width, float height, const ic::Color &color = { 255, 255, 255 });
        void draw_line(ic::Batch &batch, float x1, float y1, float x2, float y2, const ic::Color &color = { 255, 255, 255 });
        
        void draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, const ic::Color &color = { 255, 255, 255 }); 
        void draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<unsigned int> indices, const ic::Color &color = { 255, 255, 255 }); 
        
        void draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<ic::Color> vertexColors, const ic::Color &color = { 255, 255, 255 }); 
        void draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<unsigned int> indices, std::vector<ic::Color> vertexColors, const ic::Color &color = { 255, 255, 255 }); 
        
        void draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<ic::Color> vertexColors, std::vector<ic::Vec2f> vertexTexCoords, const ic::Color &color = { 255, 255, 255 }); 
        void draw_vertices(ic::Batch &batch, std::vector<ic::Vec2f> vertices, std::vector<unsigned int> indices, std::vector<ic::Color> vertexColors, std::vector<ic::Vec2f> vertexTexCoords, const ic::Color &color = { 255, 255, 255 }); 

        void draw_string_monospace(ic::Batch &batch, ic::TextAtlas &textAtlas, const std::string &text, float x, float y, float h, float advance, const ic::Color &color = ic::Colors::white);
        void draw_string(ic::Batch &batch, ic::TextAtlas &textAtlas, const std::string &text, float x, float y, float h, const ic::Color &color = ic::Colors::white);

        float calculate_width(ic::TextAtlas &atlas, const std::string &text, float h);

        void tint(const ic::Color &to, float amount);
            
        namespace {
            float tinting = 0.0f;
            ic::Color tintColor = { 255, 255, 255 };
        };
    };
}
#endif
