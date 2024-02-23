#include <Icosahedron/graphics/Images.h>
#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/util/Noise.h>

ic::Image ic::Images::grayscale(ic::Image source) {
    ic::Image result = ic::Image(source.get_width(), source.get_height());

    source.each([&](int x, int y) {
        ic::Color sourceColor = source.pixel_at_unsafe(x, y);
        uint8_t average = (uint8_t) 0.2126f * sourceColor.r + 0.7152f * sourceColor.g + 0.0722f * sourceColor.b;
        
        result.set_pixel_unsafe(x, y, { average, average, average });
    });

    return result;
}

ic::Image ic::Images::perlin(int width, int height, const ic::ImageNoiseParameters &params) {
    ic::Image result = ic::Image(width, height);

    result.each([&](int x, int y) {
        float sum = 0.0f, strength = 1.0f, scale = 1.0f;

        for (int i = 0; i < params.octaves; i++) {
            sum += strength * ic::Noise::perlin_2D(x * params.scaling * scale, y * params.scaling * scale, false);
            scale *= 2.0f;
            strength *= params.persistence;
        }
        uint8_t noise = (uint8_t) (ic::Mathf::clamp(sum, 0.0f, 1.0f) * 255);

        result.set_pixel_unsafe(x, y, { noise, noise, noise });
    });

    return result;
}

ic::Image ic::Images::perlin_solid(int width, int height, const ic::ImageNoiseParameters &params) {
    ic::Image result = ic::Image(width, height);

    result.each([&](int x, int y) {
        float sum = 0.0f, strength = 1.0f, scale = 1.0f;

        for (int i = 0; i < params.octaves; i++) {
            sum += strength * ic::Noise::perlin_2D(x * params.scaling * scale, y * params.scaling * scale, false);
            scale *= 2.0f;
            strength *= params.persistence;
        }
        // Maps to normalized values from [0, 1]
        sum += 1.0f;
        sum *= 0.5f;
        
        uint8_t noise = (uint8_t) (ic::Mathf::clamp(sum, 0.0f, 1.0f) * 255);

        result.set_pixel_unsafe(x, y, { noise, noise, noise });
    });

    return result;
}