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

            void add_texture(const std::string &fileName);

            void use(int index = 0);
            void unuse();

            void dispose();

        private:
            void setup(int textureSize, unsigned int numberOfTextures);

        private:
            GLuint textureIndex = 0;
            GLuint texturesUsed = 0;

            GLsizei numberOfTextures = 0;

            GLsizei textureSize;
    };
}
#endif