#ifndef IC_CUBEMAP_LOADER_H
#define IC_CUBEMAP_LOADER_H

#include <string>
#include <vector>

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_image.h>

#include <Icosahedron/graphics/gl/Cubemap.h>



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


            ic::Cubemap load_png(const std::vector<std::string> &filePaths, bool gammaCorrection = false);
            ic::Cubemap load_bmp(const std::vector<std::string> &filePaths, bool gammaCorrection = false);

        private:
            GLenum map_to_texture_format(uint32_t format, bool gammaCorrection = false);
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