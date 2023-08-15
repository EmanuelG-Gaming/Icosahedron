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
            
            /* By default, FreeType 2 would specify the FULL path to the file. 
             * It's not a relative path. */
            void add_atlas(const char *name, const char *fileName, int height);
            TextAtlas *find_atlas(const char *name);
              
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
        std::map<const char*, TextAtlas*> atlases;
    };
}
#endif