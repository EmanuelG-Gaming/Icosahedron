#include <Icosahedron/math/geom/Bezier.h>

template<std::size_t dims>
ic::Vector<float, dims> ic::Bezier::quadratic(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, ic::Vector<float, dims> &P3, float t) {
    float inverse = (1 - t);
    ic::Vector<float, dims> result = (P1 * inverse + P2 * t) * inverse + (P2 * inverse + P3 * t) * t;
    return result;
}


template<std::size_t dims>
ic::Vector<float, dims> ic::Bezier::quadratic_derivative(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, ic::Vector<float, dims> &P3, float t) {
    ic::Vector<float, dims> result = (P2 - P1) * 2 * (1 - t) + (P3 - P2) * 2 * t;
    return result;
}

ic::Vec2 ic::Bezier::quadratic2(ic::Vec2 &P1, ic::Vec2 &P2, ic::Vec2 &P3, float t) {
    return ic::Bezier::quadratic<2>(P1, P2, P3, t);
}
ic::Vec3 ic::Bezier::quadratic3(ic::Vec3 &P1, ic::Vec3 &P2, ic::Vec3 &P3, float t) {
    return ic::Bezier::quadratic<3>(P1, P2, P3, t);
}

ic::Vec2 ic::Bezier::quadratic_derivative2(ic::Vec2 &P1, ic::Vec2 &P2, ic::Vec2 &P3, float t) {
    return ic::Bezier::quadratic_derivative<2>(P1, P2, P3, t);
}

ic::Vec3 ic::Bezier::quadratic_derivative3(ic::Vec3 &P1, ic::Vec3 &P2, ic::Vec3 &P3, float t) {
    return ic::Bezier::quadratic_derivative<3>(P1, P2, P3, t);
} 