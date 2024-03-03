#include <Icosahedron/math/geom/Bezier.h>


template<std::size_t dims>
ic::Vector<float, dims> ic::Bezier::linear(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, float t) {
    ic::Vector<float, dims> result = P1 * (1 - t) + P2 * t;
    return result;
}
template<std::size_t dims>
ic::Vector<float, dims> ic::Bezier::linear_derivative(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, float t) {
    ic::Vector<float, dims> result = P2 - P1;
    return result;
}



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



template<std::size_t dims>
ic::Vector<float, dims> ic::Bezier::cubic(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, ic::Vector<float, dims> &P3, ic::Vector<float, dims> &P4, float t) {
    float inverse = (1 - t);
    //ic::Vector<float, dims> lP1 = (P1 * inverse + P2 * t) * inverse + (P2 * inverse + P3 * t) * t;
    //ic::Vector<float, dims> lP2 = (P2 * inverse + P3 * t) * inverse + (P3 * inverse + P4 * t) * t;
    ic::Vector<float, dims> result = ((P1 * inverse + P2 * t) * inverse + (P2 * inverse + P3 * t) * t) * inverse + 
                                    ((P2 * inverse + P3 * t) * inverse + (P3 * inverse + P4 * t) * t) * t;

    return result;
}
template<std::size_t dims>
ic::Vector<float, dims> ic::Bezier::cubic_derivative(ic::Vector<float, dims> &P1, ic::Vector<float, dims> &P2, ic::Vector<float, dims> &P3, ic::Vector<float, dims> &P4, float t) {
    float inverse = (1 - t);
    ic::Vector<float, dims> result = 3 * inverse * inverse * (P2 - P1) + 6 * inverse * (P3 - P1) + 3 * t * t * (P4 - P3);

    return result;
}