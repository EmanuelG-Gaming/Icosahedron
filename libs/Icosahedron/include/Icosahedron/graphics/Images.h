#ifndef IC_IMAGES_H
#define IC_IMAGES_H

#include <Icosahedron/graphics/Image.h>


namespace ic {
    struct ImageNoiseParameters {
        int octaves = 1;
        float persistence = 0.55f;
        float scaling = 0.1f;

        ImageNoiseParameters() {

        }
    };

    namespace Images {
        ic::Image grayscale(ic::Image source);
        ic::Image perlin(int width, int height, const ic::ImageNoiseParameters &params = ic::ImageNoiseParameters());

        /** @brief Applies Perlin noise over an image, except the range of values before clamping
         *  is roughly in the interval [0, 1], instead of [-1, 1].
        */
        ic::Image perlin_solid(int width, int height, const ic::ImageNoiseParameters &params = ic::ImageNoiseParameters());
    };
}
#endif