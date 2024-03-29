#ifndef IC_COLORS_H
#define IC_COLORS_H

#include <Icosahedron/graphics/Color.h>

namespace ic {
    /** @brief Color presets. */
    namespace Colors {
        const ic::Color
            red = { 255, 0, 0 },
            green = { 0, 255, 0 },
            blue = { 0, 0, 255 },

            yellow = { 255, 255, 0 },
            magenta = { 255, 0, 255 },
            cyan = { 0, 255, 255 },

            black = { 0, 0, 0 },
            white = { 255, 255, 255 },

            gray = { 80, 80, 80 },
            lightGray = { 130, 130, 130 },
            
            // Fully transparent color
            fTransparent = { 0, 0, 0, 255 },
            
            // Same as black
            fOpaque;
    };
}

#endif