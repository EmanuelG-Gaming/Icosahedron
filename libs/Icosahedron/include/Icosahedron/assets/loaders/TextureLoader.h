#ifndef IC_TEXTURE_LOADER_H
#define IC_TEXTURE_LOADER_H

#include <string>

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>

#include <Icosahedron/math/Mathf.h>
#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/Image.h>


namespace ic {

    class TextureLoader {
        public:
            static TextureLoader& get() {
                static TextureLoader ins;
                return ins;
            }


            /**  @param gammaCorrection Whether or not the texture has sRGB pixels. */
            ic::Texture load_png(const std::string &filePath, ic::TextureParameters parameters = ic::TextureParameters(), bool gammaCorrection = false);
            
            /**  @param gammaCorrection Whether or not the texture has sRGB pixels. */
            ic::Texture load_bmp(const std::string &filePath, ic::TextureParameters parameters = ic::TextureParameters(), bool gammaCorrection = false);


            /** @brief Loads a texture based on an RGB image. 
             *  @note The source image won't get disposed automatically, yet the underlying SDL_Surface will be. 
             *  @param gammaCorrection Whether or not the texture has sRGB pixels. */
            ic::Texture load(ic::Image &image, ic::TextureParameters parameters = ic::TextureParameters(), bool gammaCorrection = false);

            /** @brief Converts a 32-bit encoding into a format that can be used by OpenGL.
             *  @param gammaCorrection Whether or not the format is in the non-linear sRGB space.
            */
            GLenum map_to_texture_format(uint32_t format, bool gammaCorrection = false);

        private:
            ic::Texture load_texture(const void *data, int width, int height, GLenum internalFormat, GLenum format, const ic::TextureParameters &parameters);

        private:
            TextureLoader() {}
            ~TextureLoader() {}

        public:
            TextureLoader(TextureLoader const&) = delete;
            void operator = (TextureLoader const&) = delete;
    };
}

#endif