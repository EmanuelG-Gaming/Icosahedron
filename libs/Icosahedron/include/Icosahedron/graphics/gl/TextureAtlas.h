#ifndef IC_TEXTURE_ATLAS_H
#define IC_TEXTURE_ATLAS_H

#include <vector>
#include <map>
#include <string>

#include <glad/glad.h>


#include <Icosahedron/graphics/Image.h>
#include <Icosahedron/graphics/ImageIO.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>


namespace ic {
    struct AtlasEntry {
        float x, y;
        float width, height;

        float u, v, u2, v2;
    };

    class TextureAtlas {
        public:
            TextureAtlas(int width, int height);
            TextureAtlas();


            void use();
            void unuse();
            void dispose();



            ic::AtlasEntry add_entry(const char *location, int w, int h, GLenum format, const void *pixels);
            ic::AtlasEntry add_entry(const char *location, ic::Image &image);
            ic::AtlasEntry add_entry(const std::string &location, const std::string fileName);


            ic::AtlasEntry add_entry(const char *location, const char *fileName);



            /** @brief Loads multiple images based on the format: { image_alias1, image_file_name1, image_alias2, image_file_name2, ... }*/
            void add_entries(const std::vector<std::string> &from);

            AtlasEntry& get_entry(const std::string &location);

            AtlasEntry& get_entry(int index);

            float get_width();
            float get_height();

        private:
            void initialize();

        private:
            GLuint textureIndex;

            int atlasWidth, atlasHeight;
            int lastX, lastY;
            int lastRowHeight;

            std::map<std::string, AtlasEntry> entries;
    };
}
#endif