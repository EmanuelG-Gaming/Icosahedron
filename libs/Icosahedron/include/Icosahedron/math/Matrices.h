#ifndef IC_MATRICES_H
#define IC_MATRICES_H

#include <Icosahedron/math/Matrix.h>

namespace ic {
    // Common matrix types

    using Mat2x2 = ic::Matrix<float, 2, 2>;
    using Mat3x3 = ic::Matrix<float, 3, 3>;
    using Mat4x4 = ic::Matrix<float, 4, 4>;
}
#endif