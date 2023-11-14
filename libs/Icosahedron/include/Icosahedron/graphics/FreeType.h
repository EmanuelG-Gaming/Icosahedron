#ifndef IC_FREETYPE_H
#define IC_FREETYPE_H

#include <map>
#include <stdexcept>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Icosahedron/graphics/TextAtlas.h>


namespace ic {
    /** @brief The starting point for the FreeType 2 font renderer. */
    class FreeType {
        public:
            static FreeType& get() {
                static FreeType ins;
                return ins;
            }
         
            void load();
            
            /** @brief Loads a TrueType file from a relative path.
             *  @note By default, FreeType would choose an absolute directory, rather than a relative one. */
            void add_atlas(const std::string &name, const std::string &relativeFile, int height);

            TextAtlas *find_atlas(const std::string &name);
            
            void dispose();
              
     private:
        FreeType() {}
        ~FreeType() {}
     public:
        FreeType(FreeType const&) = delete;
        void operator = (FreeType const&) = delete;
        
     private:
        FT_Library library;
        FT_Error errorHandler;
        std::map<std::string, TextAtlas*> atlases;
    };
}
#endif