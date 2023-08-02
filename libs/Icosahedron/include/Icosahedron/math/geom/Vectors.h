#ifndef IC_VECTORS_H
#define IC_VECTORS_H

#include <Icosahedron/math/geom/Vector.h>

#include <stdint.h>


namespace ic {
    // Common vector types

    using Vec2f = ic::Vector<float, 2>;
    using Vec3f = ic::Vector<float, 3>;
    using Vec4f = ic::Vector<float, 4>;

    using Vec2i = ic::Vector<int, 2>;
    using Vec3i = ic::Vector<int, 3>;
    using Vec4i = ic::Vector<int, 4>;

    using Vec2b = ic::Vector<uint8_t, 2>;
    using Vec3b = ic::Vector<uint8_t, 3>;
    using Vec4b = ic::Vector<uint8_t, 4>;
}
#endif