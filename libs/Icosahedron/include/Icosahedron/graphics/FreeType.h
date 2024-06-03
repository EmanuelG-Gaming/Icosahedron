#ifndef IC_FREETYPE_H
#define IC_FREETYPE_H

#include <map>
#include <stdexcept>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Icosahedron/graphics/TextAtlas.h>


namespace ic {
    // @brief The starting point for the FreeType 2 font renderer.
    namespace FreeType {
        void load();
            
        /** @brief Loads a TrueType file from a relative path.
         *  @note By default, FreeType would choose an absolute directory, rather than a relative one.
         */
        TextAtlas add_atlas(const std::string &name, const std::string &relativeFile, int height);
        
        TextAtlas &find_atlas(const std::string &name);
        
        void dispose();
              
        namespace {
            FT_Library library;
            FT_Error errorHandler;
            std::map<std::string, TextAtlas> atlases;
        };
    };
}
#endif
