#ifndef IC_FREETYPE_H
#define IC_FREETYPE_H

#include <stdio.h>
#include <stdlib.h>

#include <Icosahedron/graphics/TextAtlas.h>


namespace ic {
    namespace FreeType {
        ic::TextAtlas load_atlas(const char *filePath);
    };
}
#endif
