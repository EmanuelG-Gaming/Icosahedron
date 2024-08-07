#ifndef IC_TEXTURE_LOADER_H
#define IC_TEXTURE_LOADER_H

#include <string>

#include <glad/glad.h>

#include <Icosahedron/math/Mathf.h>
#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/Image.h>


namespace ic {
    namespace TextureLoader {
        /**  @param gammaCorrection Whether or not the texture has sRGB pixels. */
        ic::Texture load(const char *filePath, ic::TextureParameters parameters = ic::TextureParameters(), bool gammaCorrection = false);
        ic::Texture load(const std::string &filePath, ic::TextureParameters parameters = ic::TextureParameters(), bool gammaCorrection = false);
        
        /** @brief Loads a texture based on an RGB image. 
         *  @note The source image won't get disposed automatically, yet the underlying SDL_Surface will be. 
         *  @param gammaCorrection Whether or not the texture has sRGB pixels. */
        ic::Texture load(ic::Image &image, ic::TextureParameters parameters = ic::TextureParameters(), bool gammaCorrection = false);

        /** @brief Converts a 32-bit encoding into a format that can be used by OpenGL.
         *  @param gammaCorrection Whether or not the format is in the non-linear sRGB space.
        */
        GLenum map_to_texture_format(int numberOfChannels, bool gammaCorrection = false);

        namespace {
            ic::Texture load_texture(const void *data, int width, int height, GLenum internalFormat, GLenum format, const ic::TextureParameters &parameters);
        };
    };
}

#endif