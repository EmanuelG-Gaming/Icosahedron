#ifndef IC_TEXT_ATLAS_H
#define IC_TEXT_ATLAS_H

#include <array>

#include <glad/glad.h>



namespace ic {
    /** @brief Stores information about a character (glyph) of a text atlas. */
    struct CharacterInfo {
        float u0, v0, u1, v1;
        float p0x, p0y, p1x, p1y;

        float advance;
    };

    class TextAtlas {
        public:
            int atlasWidth, atlasHeight;
            CharacterInfo *characters;

            int firstGlyph, glyphCount;
            int fontHeight;
            
        public:
            TextAtlas();
            
            void add_empty_texture();
            void initialize_texture(const void *bitmap);

            void use();
            void dispose();

            /** @brief Copies a bitmap (usually from a glyph) to the texture data, using the GL_RED component.  */
            void blit_glyph(float u1, float v1, float width, float height, const void *pixels);


            ic::CharacterInfo &glyph_at(int index);
            
            float get_width();
            float get_height();

        private:
            GLuint textureIndex;
    };
}
#endif
