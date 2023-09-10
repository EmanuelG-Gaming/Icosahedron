#ifndef IC_IMAGE_IO_H
#define IC_IMAGE_IO_H

#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <Icosahedron/files/File.h>
#include <Icosahedron/graphics/Image.h>


namespace ic {
    class ImageIO {
        public:
            static ImageIO& get() {
                static ImageIO ins;
                return ins;
            }

            ic::Image read_png(ic::File file);
            ic::Image read_bmp(ic::File file);
            ic::Image read_ppm(ic::File file);
        
            void write_png(ic::File file, ic::Image &image);
            void write_bmp(ic::File file, ic::Image &image);
            void write_ppm(ic::File file, ic::Image &image);

            SDL_Surface *to_surface(ic::Image &image);

        private:
            ImageIO() {}
            ~ImageIO() {}

        public:
            ImageIO(ImageIO const&) = delete;
            void operator = (ImageIO const&) = delete;
    };
}
#endif