#ifndef IC_TEXT_ATLAS_H
#define IC_TEXT_ATLAS_H

#include <array>

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H


namespace ic {
    /** @brief Stores information about a character of a text atlas. */
    struct CharacterInfo {
        float advX, advY;
    
        float bitmapWidth, bitmapHeight;
        float bitmapLeft, bitmapTop;
    
        float offsetX = 0.0f;
    };

    /** @brief A text atlas stores ASCII bitmap glyphs in a horizontal order. */
    class TextAtlas {
        public:
            TextAtlas();
            TextAtlas(FT_Face font);

            void load();

            void use();
            void dispose();

            std::array<CharacterInfo, 128> &get_characters();
            float get_width();
            float get_height();

        protected:
            void adjust_size();
            void add_empty_texture();

        private:
            FT_Face font;
            FT_GlyphSlot glyph;
            GLuint textureIndex;
         
            int atlasWidth, atlasHeight;
            std::array<CharacterInfo, 128> characters; // Currently the stores ASCII character range
    };
}
#endif