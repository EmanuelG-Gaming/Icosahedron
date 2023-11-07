#ifndef IC_TEXTURE_LOADER_H
#define IC_TEXTURE_LOADER_H

#include <string>

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <Icosahedron/math/Mathf.h>
#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/Image.h>


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

    class TextureLoader {
        public:
            static TextureLoader& get() {
                static TextureLoader ins;
                return ins;
            }


            /**  @param gammaCorrection Whether or not the texture has sRGB pixels. */
            ic::Texture *load_png(const std::string &filePath, ic::TextureParameters parameters = ic::TextureParameters(), bool gammaCorrection = false);
            
            /**  @param gammaCorrection Whether or not the texture has sRGB pixels. */
            ic::Texture *load_bmp(const std::string &filePath, ic::TextureParameters parameters = ic::TextureParameters(), bool gammaCorrection = false);


            /** @brief Loads a texture based on an RGB image. 
             *  @note The source image won't get disposed automatically, yet the underlying SDL_Surface will be. 
             *  @param gammaCorrection Whether or not the texture has sRGB pixels. */
            ic::Texture *load(ic::Image &image, ic::TextureParameters parameters = ic::TextureParameters(), bool gammaCorrection = false);

            /** @brief Converts a 32-bit encoding into a format that can be used by OpenGL.
             *  @param gammaCorrection Whether or not the format is in the non-linear sRGB space.
            */
            GLenum map_to_texture_format(uint32_t format, bool gammaCorrection = false);

        private:
            ic::Texture *load_texture(int width, int height, GLenum format, const void *data, const ic::TextureParameters &parameters);

        private:
            TextureLoader() {}
            ~TextureLoader() {}

        public:
            TextureLoader(TextureLoader const&) = delete;
            void operator = (TextureLoader const&) = delete;
    };
}

#endif