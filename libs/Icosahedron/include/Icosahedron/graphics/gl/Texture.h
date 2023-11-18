#ifndef IC_TEXTURE_H
#define IC_TEXTURE_H

#include <glad/glad.h>

#include <Icosahedron/graphics/gl/GL.h>


namespace ic {
    /** @brief An OpenGL wrapper for a texture.
     * A texture is bound via the use() function, in order to be applied to geometry. */
    class Texture {
        public:
            Texture(ic::GLTextureTypes type = ic::GLTextureTypes::INVALID_TEXTURE);

            void use(int index = 0);
            void unuse();

            void dispose();
            
            ic::GLTextureTypes get_type();

        private:
            void setup(const ic::GLTextureTypes &textureType);
            
        private:
            GLuint textureIndex = 0;
            ic::GLTextureTypes type;
    };
}

#endif