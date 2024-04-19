#include <Icosahedron/math/Interpolation.h>
#include <Icosahedron/math/Mathf.h>

float ic::Interpolation::linear(float x) {
    return x;
}
float ic::Interpolation::quadratic(float x) {
    return x * x;
}
float ic::Interpolation::cubic(float x) {
    return x * x * x;
}
float ic::Interpolation::quartic(float x) {
    return x * x * x * x;
}
float ic::Interpolation::quintic(float x) {
    return x * x * x * x * x;
}

float ic::Interpolation::square_root(float x) {
    return sqrt(x);
}


float ic::Interpolation::exponential(float x) {
    return ic::Mathf::exponential(x) * (4.0f / 7.0f);
}

float ic::Interpolation::smoothstep(float x) {
    return (3.0f - x * 2.0f) * x * x;
}
float ic::Interpolation::smootherstep(float x) {
    return x * x * x * (x * (6.0f * x - 15.0f) + 10.0f);
}