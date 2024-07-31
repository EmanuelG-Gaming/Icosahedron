#ifndef IC_FONT_LOADER_H
#define IC_FONT_LOADER_H

#include <stdio.h>
#include <stdlib.h>

#include <Icosahedron/graphics/TextAtlas.h>


namespace ic {
    namespace FontLoader {
        /** @brief Loads a font containing most ASCII characters, in a horizontal array.
         *  @param filePath A relative or absolute directory path that points to a file with a format like .ttf.
         *  @param fontWidth The width of the text atlas.
         *  @param fontHeight The average size of each character.
         *  @author Slightly modified from https://stackoverflow.com/questions/51276586/how-to-render-text-in-directx9-with-stb-truetype.
         */
        ic::TextAtlas load(const char *filePath, int fontWidth = 4096, int fontHeight = 64, int lineHeight = 32);
    };
}
#endif
