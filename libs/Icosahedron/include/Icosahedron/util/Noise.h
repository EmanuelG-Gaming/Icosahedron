#ifndef IC_NOISE_H
#define IC_BOISE_H

#include <cstdint>
#include <cstring>

#include <Icosahedron/math/Mathf.h>
#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/Interpolation.h>


namespace ic {
    namespace Noise {
        namespace {
            uint8_t permutations[256 * 2];
            bool loaded = false;

            void load_permutation_lookup();

            ic::Vec2f get_2D_vector_from_permutation(uint8_t value);
            float get_value_from_permutation(uint8_t value);
        };


        void init();

        /** @brief Calculates the Perlin noise at a location.
         *  @return A value in the interval [0, 1] if normalized, and [-1, 1] otherwise. */
        float perlin_1D(float position, bool normalized = true);

        /** @brief Calculates the Perlin noise at an (x, y) position.
         *  @return A value in the interval [0, 1] if normalized, and [-1, 1] otherwise. */
        float perlin_2D(float x, float y, bool normalized = true);
    };
}
#endif