#ifndef IC_TEXT_ATLAS_H
#define IC_TEXT_ATLAS_H

#include <array>

#include <glad/glad.h>

#include <stb_truetype.h>


namespace ic {
    /** @brief Stores information about a character of a text atlas. */
    struct CharacterInfo {
        float advX, advY;
    
        float bitmapWidth, bitmapHeight;
        float bitmapLeft, bitmapTop;
    
        float offsetX = 0.0f;
    };

    class TextAtlas {
        public:
            TextAtlas();
            
            void add_empty_texture();

            void use();
            void dispose();

            std::array<CharacterInfo, 128> &get_characters();
            float get_width();
            float get_height();

        private:
            GLuint textureIndex;
         
            int atlasWidth, atlasHeight;
            std::array<CharacterInfo, 128> characters; // Currently stores ASCII character range
    };
}
#endif
