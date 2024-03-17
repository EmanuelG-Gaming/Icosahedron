#ifndef IC_MATRICES_H
#define IC_MATRICES_H

#include <Icosahedron/math/Matrix.h>

namespace ic {
    // Common matrix types

    using Mat2x2 = ic::Matrix<float, 2, 2>;
    using Mat3x3 = ic::Matrix<float, 3, 3>;
    using Mat4x4 = ic::Matrix<float, 4, 4>;

    using Mat2x2i = ic::Matrix<int, 2, 2>;
    using Mat3x3i = ic::Matrix<int, 3, 3>;
    using Mat4x4i = ic::Matrix<int, 4, 4>;

    using Mat2 = Mat2x2;
    using Mat3 = Mat3x3;
    using Mat4 = Mat4x4;
}
#endif