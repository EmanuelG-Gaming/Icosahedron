#ifndef IC_TEXT_ATLAS_H
#define IC_TEXT_ATLAS_H

#include <array>

#include <glad/glad.h>



namespace ic {
    /** @brief Stores information about a character (glyph) of a text atlas. */
    struct CharacterInfo {
        // Just for easier access
        float width, height;

        // The bounding box coordinates relative to the text atlas (in the interval [0, 1]) 
        float u1, v1, u2, v2;

        float shift;
    };

    class TextAtlas {
        public:
            int atlasWidth, atlasHeight;
            std::array<CharacterInfo, 128> characters;
            
        public:
            TextAtlas();
            
            void add_empty_texture();
            void initialize_texture(const void *bitmap);

            void use();
            void dispose();

            /** @brief Copies a bitmap (usually from a glyph) to the texture data, using the GL_RED component.  */
            void blit_glyph(float u1, float v1, float width, float height, const void *pixels);


            std::array<CharacterInfo, 128> &get_characters();
            float get_width();
            float get_height();

        private:
            GLuint textureIndex;
    };
}
#endif
