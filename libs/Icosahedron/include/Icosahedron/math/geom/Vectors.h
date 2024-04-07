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

    // Same as ic::Vec2f
    using Vec2 = Vec2f;
    // Same as ic::Vec3f
    using Vec3 = Vec3f;
    // Same as ic::Vec4f
    using Vec4 = Vec4f;

    namespace Vectors {
        /** @brief Euclidean 2D axes. */
        const ic::Vec2 X(1.0f, 0.0f), Y(0.0f, 1.0f), negativeX(-1.0f, 0.0f), negativeY(0.0f, -1.0f);
        
        /** @brief Euclidean 3D axes. */
        const ic::Vec3 right(1.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f), front(0.0f, 0.0f, 1.0f), 
                       left(-1.0f, 0.0f, 0.0f), down(0.0f, -1.0f, 0.0f), back(0.0f, 0.0f, -1.0f);
        
        const ic::Vec2 null2D(0.0f, 0.0f);
        const ic::Vec3 null3D(0.0f, 0.0f, 0.0f);
    }
}
#endif