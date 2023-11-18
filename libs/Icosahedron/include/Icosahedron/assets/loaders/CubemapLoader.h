#ifndef IC_CUBEMAP_LOADER_H
#define IC_CUBEMAP_LOADER_H

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <Icosahedron/graphics/gl/Texture.h>


namespace ic {
    struct CubemapFaceInformation {
        int width, height;
        int faceIndex;

        GLenum format;
        const void *data;
    };

    class CubemapLoader {
        public:
            static CubemapLoader& get() {
                static CubemapLoader ins;
                return ins;
            }


            ic::Texture load_png(const std::vector<std::string> &filePaths);
            ic::Texture load_bmp(const std::vector<std::string> &filePaths);

        private:
            GLenum map_to_texture_format(uint32_t format);
            void load_cubemap_face(const ic::CubemapFaceInformation &data);

        private:
            CubemapLoader() {}
            ~CubemapLoader() {}

        public:
            CubemapLoader(CubemapLoader const&) = delete;
            void operator = (CubemapLoader const&) = delete;
    };
}
#endif