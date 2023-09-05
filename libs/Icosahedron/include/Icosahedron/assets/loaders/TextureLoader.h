#ifndef IC_TEXTURE_LOADER_H
#define IC_TEXTURE_LOADER_H

#include <string>

#include <GL/glew.h>

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


            ic::Texture *load(const std::string &filePath, ic::TextureParameters parameters = ic::TextureParameters());

            /** @brief Loads a texture based on an RGB image. 
             *  @note The source image won't get disposed automatically, yet the underlying SDL_Surface will be. */
            ic::Texture *load(ic::Image &image, ic::TextureParameters parameters = ic::TextureParameters());

        private:
            SDL_Surface *load_surface(const std::string &path);
            GLenum map_to_texture_format(uint32_t format);
        
        private:
            TextureLoader() {}
            ~TextureLoader() {}

        public:
            TextureLoader(TextureLoader const&) = delete;
            void operator = (TextureLoader const&) = delete;
    };
}

#endif