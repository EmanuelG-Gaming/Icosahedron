#ifndef IC_FONT_LOADER_H
#define IC_FONT_LOADER_H

#include <stdio.h>
#include <stdlib.h>

#include <Icosahedron/graphics/TextAtlas.h>


namespace ic {
    namespace FontLoader {
        /** @brief Loads a font page containing most ASCII characters to a mostly horizontal image.
         *  @author Slightly modified from https://stackoverflow.com/questions/51276586/how-to-render-text-in-directx9-with-stb-truetype.
         */
        ic::TextAtlas load(const char *filePath, int atlasWidth = 4096, int atlasHeight = 4096, int characterSize = 128, int glyphOversamplingX = 5, int glyphOversamplingY = 5, int firstCharacter = 0, int numberOfCharacters = 128);
    };
}
#endif
