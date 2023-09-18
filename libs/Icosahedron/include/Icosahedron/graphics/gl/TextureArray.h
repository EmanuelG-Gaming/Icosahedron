#ifndef IC_TEXTURE_ARRAY_H
#define IC_TEXTURE_ARRAY_H

#include <string>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


namespace ic {
    class TextureArray {
        public:
            TextureArray(int textureWidth, int textureHeight, unsigned int numberOfTextures);
            TextureArray(int textureSize, unsigned int numberOfTextures);

            void add_texture(const std::string &fileName);

            void use(int index = 0);
            void unuse();

            void dispose();

        private:
            void setup(int textureWidth, int textureHeight, unsigned int numberOfTextures);

        private:
            GLuint textureIndex = 0;
            GLuint texturesUsed = 0;

            GLsizei numberOfTextures = 0;

            GLsizei textureWidth;
            GLsizei textureHeight;
    };
}
#endif