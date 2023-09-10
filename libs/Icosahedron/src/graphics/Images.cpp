#include <Icosahedron/graphics/Images.h>
#include <Icosahedron/graphics/Color.h>


ic::Image ic::Images::grayscale(ic::Image source) {
    ic::Image result = ic::Image(source.get_width(), source.get_height());

    source.each([&](int x, int y) {
        ic::Color sourceColor = source.pixel_at_unsafe(x, y);
        uint8_t average = (uint8_t) 0.2126f * sourceColor.r + 0.7152f * sourceColor.g + 0.0722f * sourceColor.b;
        
        result.set_pixel_unsafe(x, y, { average, average, average });
    });

    return result;
}