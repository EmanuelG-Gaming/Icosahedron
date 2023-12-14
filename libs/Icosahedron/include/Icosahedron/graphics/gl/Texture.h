#ifndef IC_TEXTURE_H
#define IC_TEXTURE_H

#include <glad/glad.h>

#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/math/geom/Vectors.h>


namespace ic {
    struct TextureParameters {
        bool usesMipmapping = false;

        ic::GLTextureFilter minFilter = ic::TEXTURE_FILTER_NEAREST;
        ic::GLTextureFilter magFilter = ic::TEXTURE_FILTER_NEAREST;
        
        ic::GLTextureWrap wrapU = ic::TEXTURE_WRAP_REPEAT;
        ic::GLTextureWrap wrapV = ic::TEXTURE_WRAP_REPEAT;

        TextureParameters() {
            
        }
    };

    /** @brief An OpenGL wrapper for a two-dimensional texture.
     * A texture is bound via the use() function, in order to be applied to geometry. */
    class Texture {
        public:
            Texture();

            ic::Texture &setup();
            ic::Texture &setup_from_array(const void *data, int width, int height, GLenum internalFormat, GLenum format, const ic::TextureParameters &parameters = ic::TextureParameters());

            ic::Vec2i get_dimensions();

            /** @brief Overrides the content of the image. Useful for dynamically changing the whole texture. 
             *  @param content Indeterminate data to be sent
             *  @param format The original pixel format of the image
            */
            ic::Texture &set_pixel_content(const void *content, GLenum format);

            void use(int index = 0);
            void unuse();

            void dispose();
        
        private:
            GLuint textureIndex = 0;
    };
}

#endif