#ifndef IC_TEXTURE_ARRAY_H
#define IC_TEXTURE_ARRAY_H

#include <string>

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_image.h>


namespace ic {
    class TextureArray {
        public:
            TextureArray();
            TextureArray(int textureSize, unsigned int numberOfTextures);

            /** @note This will empty the array of textures. */
            void load(int textureSize, unsigned int numberOfTextures);


            void add_texture(const std::string &fileName);

            void use(int index = 0);
            void unuse();

            void dispose();

            
            unsigned int get_textures_used();
            unsigned int get_texture_capacity();
            size_t get_texture_width();

        private:
            GLuint textureIndex = 0;
            GLuint texturesUsed = 0;

            GLsizei numberOfTextures = 0;

            GLsizei textureSize;
    };
}
#endif