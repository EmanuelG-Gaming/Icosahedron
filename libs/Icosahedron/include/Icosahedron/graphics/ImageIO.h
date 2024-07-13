#ifndef IC_IMAGE_IO_H
#define IC_IMAGE_IO_H

#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>

#include <SDL.h>

#include <Icosahedron/graphics/Image.h>


namespace ic {
    namespace ImageIO {
        //ic::Image read_png(const std::string &fileName);
        ic::Image read_bmp(const std::string &fileName);
        ic::Image read_ppm(const std::string &fileName);
    
        //void write_png(const std::string &fileName, const ic::Image &image);
        void write_bmp(const std::string &fileName, const ic::Image &image);
        void write_ppm(const std::string &fileName, const ic::Image &image);

        SDL_Surface *to_surface(const ic::Image &image);
        ic::Image to_image(SDL_Surface *surface);

        /** @brief Checks every pixel of an image to see if it has an alpha value greater than 0. */
        bool image_transparent(const ic::Image &source);
    };
}
#endif