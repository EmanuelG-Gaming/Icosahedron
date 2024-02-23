#include <Icosahedron/math/Mathf.h>


float ic::Mathf::to_radians(float degrees) {
    return degrees * degToRad;
}
float ic::Mathf::to_degrees(float radians) {
    return radians * radToDeg;
}


float ic::Mathf::sinf(float radians) {
    return sin(radians);
}
float ic::Mathf::cosf(float radians) {
    return cos(radians);
}


float ic::Mathf::absinf(float radians) {
    return abs(sinf(radians));
}
float ic::Mathf::abcosf(float radians) {
    return abs(cosf(radians));
}


float ic::Mathf::positive_sinf(float radians) {
    return (sinf(radians) + 1) * 0.5f;
}
float ic::Mathf::positive_cosf(float radians) {
    return (cosf(radians) + 1) * 0.5f;
}


float ic::Mathf::clamp(float x, float min, float max) {
    return std::max(min, std::min(max, x));
}
int ic::Mathf::clamp(int x, int min, int max) {
    return std::max(min, std::min(max, x));
}


float ic::Mathf::interpolate(float from, float to, float alpha) {
    return from * (1 - alpha) + to * alpha;
}
float ic::Mathf::interpolate_logarithmic(float from, float to, float alpha) {
    float value = ic::Mathf::interpolate(log(from), log(to), alpha);
    value = exp(value);
    return value;
}


int ic::Mathf::logarithm(int base, int exponent) {
    return (log2(exponent) / log2(base));
}
bool ic::Mathf::is_power_of(int base, int value) {
    if (base == 0) return false;
    int v = ic::Mathf::logarithm(base, value);
    return (ceil(v) == floor(v));
}