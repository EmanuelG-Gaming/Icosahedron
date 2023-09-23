#include <Icosahedron/graphics/Image.h>

using namespace ic;


Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    this->pixels = new image_t[width * height];

    for (int i = 0; i < this->width * this->height; i++) {
        this->pixels[i] = { 255, 255, 255 };
    }
}
Image::Image(int width, int height, image_t *pixelData) {
    this->width = width;
    this->height = height;
    this->pixels = pixelData;
}

void ic::Image::set_size(int width, int height) {
    this->width = width;
    this->height = height;
    this->pixels = new image_t[width * height];

    for (int i = 0; i < this->width * this->height; i++) {
        this->pixels[i] = { 255, 255, 255 };
    }
}

void ic::Image::dispose() {
    delete[] this->pixels;
}

bool ic::Image::inside(int x, int y) {
    return (x >= 0 && y >= 0 && x < this->width && y < this->height);
}

image_t ic::Image::pixel_at_unsafe(int x, int y) {
    return this->pixels[y * width + x];
}
image_t ic::Image::pixel_at(int x, int y) {
    if (!this->inside(x, y)) return { 0, 0, 0 };

    return this->pixel_at_unsafe(x, y);
}

void ic::Image::set_pixel_unsafe(int x, int y, const image_t &with) {
    this->pixels[y * width + x] = with;
}
void ic::Image::set_pixel(int x, int y, const image_t &with) {
    if (!this->inside(x, y)) return;

    set_pixel_unsafe(x, y, with);
}

void ic::Image::fill_circle(int x, int y, int radius, const image_t &with) {
    int radiusSquared = radius * radius;

    for (int j = -radius; j <= radius; j++) {
        int py = y + j;

        for (int i = -radius; i <= radius; i++) {
            int px = x + i;

            if (i*i + j*j <= radiusSquared) {
                this->set_pixel(px, py, with);
            }
        }
    }
}

void ic::Image::draw_line(int x0, int y0, int x1, int y1, const image_t &with) {
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int displacementX = x0 < x1 ? 1 : -1;
    int displacementY = y0 < y1 ? 1 : -1;

    int error = dx + dy;

    while (true) {
        this->set_pixel(x0, y0, with);
        if (x0 == x1 && y0 == y1) break;

        int twoTimesError = 2 * error;
        if (twoTimesError >= dy) {
            if (x0 == x1) break;

            error += dy;
            x0 += displacementX;
        }

        if (twoTimesError <= dx) {
            if (y0 == y1) break;

            error += dx;
            y0 += displacementY;
        }
    }
}

void ic::Image::line_triangle(int x0, int y0, int x1, int y1, int x2, int y2, const image_t &with) {
    this->draw_line(x0, y0, x1, y1, with);
    this->draw_line(x1, y1, x2, y2, with);
    this->draw_line(x2, y2, x0, y0, with);
}

void ic::Image::fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2, const image_t &with) {
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int displacementX = x0 < x1 ? 1 : -1;
    int displacementY = y0 < y1 ? 1 : -1;

    int error = dx + dy;

    while (true) {
        this->draw_line(x0, y0, x2, y2, with);
        if (x0 == x1 && y0 == y1) break;

        int twoTimesError = 2 * error;
        if (twoTimesError >= dy) {
            if (x0 == x1) break;

            error += dy;
            x0 += displacementX;
        }

        if (twoTimesError <= dx) {
            if (y0 == y1) break;

            error += dx;
            y0 += displacementY;
        }
    }
}
       
void ic::Image::each(const std::function<void(int, int)> &call) {
    for (int y = 0; y < this->get_height(); y++) {
        for (int x = 0; x < this->get_width(); x++) {
            call(x, y);
        }
    }
}

void ic::Image::convolve(const std::array<float, 9> &kernel) {
    image_t *tmpBuffer = new image_t[this->width * this->height];

    each([&](int x, int y) {
        image_t sum = { 0, 0, 0 };

        int k = 0;
        for (int j = -1; j <= 1; j++) {
            for (int i = -1; i <= 1; i++) {
                image_t surrounding = this->pixel_at(x + i, y + j) * kernel[k];
                sum = sum + surrounding;
                k++;
            }
        }

        tmpBuffer[y * this->width + x] = sum;
    });

    each([&](int x, int y) {
        this->set_pixel_unsafe(x, y, tmpBuffer[y * this->width + x]);
    });

    delete[] tmpBuffer;
}

int ic::Image::get_width() {
    return this->width;
}
int ic::Image::get_height() {
    return this->height;
}


void *ic::Image::data() {
    return this->pixels;
}