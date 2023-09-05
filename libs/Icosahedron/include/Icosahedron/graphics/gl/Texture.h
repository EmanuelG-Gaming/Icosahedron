#ifndef IC_TEXTURE_H
#define IC_TEXTURE_H

#include <vector>
#include <string>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <Icosahedron/graphics/gl/GL.h>


namespace ic {
    /** @brief An OpenGL wrapper for a texture.
     * A texture is bound using the use() function, in order to be applied to geometry. */
    class Texture {
        public:
            Texture(ic::GLTextureTypes type);

            void use(int index = 0);
            void unuse();

            void dispose();
            
            ic::GLTextureTypes get_type();

        private:
            GLuint textureIndex = 0;
            ic::GLTextureTypes type;
    };
}

#endif