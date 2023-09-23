#ifndef IC_IMAGE_H
#define IC_IMAGE_H

#include <stdexcept>
#include <cstring>
#include <functional>

#include <array>

#include <Icosahedron/graphics/Color.h>


namespace ic {
    typedef ic::Color image_t; 

    /** @brief An image represents an RGB pixel map in memory. It has a width and a height expressed in pixels,
     *  with the coordinate system starting at the top-left corner, going down in the y direction, and right in the x direction.
     *  The image's content can be changed at runtime, and written to a format via ImageIO.
     * 
     *  This surface has to be disposed when no longer needed, otherwise memory leaks would occur.
    */
    class Image {
        public:
            /** @brief Constructs a new, yet empty image. */
            Image(int width, int height);
            /** @brief Constructs a new image from raw pixel data. */
            Image(int width, int height, image_t *pixelData);

            void set_size(int width, int height);

            void dispose();


            bool inside(int x, int y);

            /** @return The pixel at integer coordinates, WITHOUT boundary checking. */
            image_t pixel_at_unsafe(int x, int y);
            /** @return The pixel at integer coordinates, with boundary checking. */
            image_t pixel_at(int x, int y);

            /** @brief Sets the pixel at the position without boundary checking. */
            void set_pixel_unsafe(int x, int y, const image_t &with);
            /** @brief Sets the pixel at the position with boundary checking. */
            void set_pixel(int x, int y, const image_t &with);


            void fill_circle(int x, int y, int radius, const image_t &with);
            
            /** @brief Bresenham's line algorithm. */
            void draw_line(int x0, int y0, int x1, int y1, const image_t &with);
            void line_triangle(int x0, int y0, int x1, int y1, int x2, int y2, const image_t &with);

            /** @brief Iterates over each pixel of the image. */
            void each(const std::function<void(int, int)> &call);

            void convolve(const std::array<float, 9> &kernel);

            int get_width();
            int get_height();

            void *data();

        private:
            int width, height;
            image_t *pixels;
    };
}
#endif