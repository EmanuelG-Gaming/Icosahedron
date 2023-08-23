#ifndef IC_TEXTURE_ATLAS_H
#define IC_TEXTURE_ATLAS_H

#include <vector>
#include <map>
#include <string>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


namespace ic {
    struct AtlasEntry {
        float x, y;
        float width, height;

        float u, v, u2, v2;
    };

    class TextureAtlas {
        public:
            TextureAtlas(int width, int height) {
                this->lastX = 0;
                this->lastY = 0;
                this->lastRowHeight = 0;

                this->atlasWidth = width;
                this->atlasHeight = height;

                this->initialize();
            }
            TextureAtlas() : TextureAtlas(2048, 2048) {}


            void use() {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, this->textureIndex);
            }
            void unuse() {
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            void dispose() {
                glDeleteTextures(1, &textureIndex);
            }


            void add_entry(const std::string &location, const std::string &fileName) {
                SDL_Surface *surface = load_surface(fileName.c_str());
                if (surface == NULL) {
                    printf("Couldn't load texture.\n");
                    return;
                }

                int width = surface->w;
                int height = surface->h;

                AtlasEntry entry;
                entry.x = this->lastX;
                entry.y = this->lastY;
                entry.width = width;
                entry.height = height;

                // Move to next row
                if (entry.x + entry.width > this->atlasWidth) {
                    entry.x = 0;
                    entry.y += this->lastRowHeight;

                    this->lastRowHeight = 0;
                    this->lastX = entry.x;
                    this->lastY = entry.y;
                }

                // Calculate row height
                if (entry.height > this->lastRowHeight) {
                    this->lastRowHeight = entry.height;
                }

                // If entry exceeded the maximum height...
                if (entry.x + entry.height > this->atlasHeight) {
                    printf("Couldn't add texture. It exceeded the maximum atlas height.\n");
                    return;
                }

                this->lastX += entry.width;
                entry.u = entry.x / (float) this->atlasWidth;
                entry.v = entry.y / (float) this->atlasHeight;
                entry.u2 = (entry.x + entry.width) / (float) this->atlasWidth;
                entry.v2 = (entry.y + entry.height) / (float) this->atlasHeight;

                glTexSubImage2D(GL_TEXTURE_2D, 0, entry.x, entry.y, entry.width, entry.height, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

                entries[location] = entry;
                printf("Texture added.\n");
            }

            /** @brief Loads multiple images based on the format: { image_alias1, image_file_name1, image_alias2, image_file_name2, ... }*/
            void add_entries(const std::vector<std::string> &from) {
                for (int i = 0; i < from.size(); i += 2) {
                    std::string alias = from.at(i);
                    std::string fileName = from.at(i + 1);

                    add_entry(alias, fileName);
                }
            }

            AtlasEntry& get_entry(const std::string &location) {
                return entries[location];
            }

            float get_width() { return atlasWidth; }
            float get_height() { return atlasHeight; }

        private:
            void initialize() {
                glActiveTexture(GL_TEXTURE0);
                glGenTextures(1, &textureIndex);

                glBindTexture(GL_TEXTURE_2D, textureIndex);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);


                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
            SDL_Surface *load_surface(const char *path) {
                SDL_Surface *img = IMG_Load(path);
                if (img == NULL) {
                    fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
                    return NULL;
                }
                return img;
            }

        private:
            GLuint textureIndex;

            int atlasWidth, atlasHeight;
            int lastX, lastY;
            int lastRowHeight;

            std::map<std::string, AtlasEntry> entries;
    };
}
#endif