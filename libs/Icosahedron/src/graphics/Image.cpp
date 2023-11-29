#include <Icosahedron/graphics/Image.h>
#include <Icosahedron/graphics/ImageIO.h>

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/Mathf.h>

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
    int ry0 = y0, ry1 = y1, ry2 = y2;
    int rx0 = x0, rx1 = x1, rx2 = x2;

    // Do vertex y sorting
    if (ry1 < ry0) { 
        std::swap(ry0, ry1); 
        std::swap(rx0, rx1); 
    }
    if (ry2 < ry0) { 
        std::swap(ry0, ry2); 
        std::swap(rx0, rx2);
    }
    if (ry2 < ry1) { 
        std::swap(ry1, ry2); 
        std::swap(rx1, rx2);
    }


    if (ry1 == ry2) {
        this->fill_bottom_triangle(rx0, ry0, rx1, ry1, rx2, ry2, with);
    } else if (ry0 == ry1) {
        this->fill_top_triangle(rx0, ry0, rx1, ry1, rx2, ry2, with);
    } else {
        ic::Vec2i v = { rx0 + (int) (((float) (ry1 - ry0) / (float)(ry2 - ry0)) * (rx2 - rx0)), ry1 };

        this->fill_bottom_triangle(rx0, ry0, rx1, ry1, v.x(), v.y(), with);
        this->fill_top_triangle(rx1, ry1, v.x(), v.y(), rx2, ry2, with);
    }
}


void ic::Image::fill_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, const image_t &with) {
    float inverseSlope1 = (x1 - x0) / (float) (y1 - y0);
    float inverseSlope2 = (x2 - x0) / (float) (y2 - y0);

    float currentX1 = x0, currentX2 = x0;
    
    for (int scanY = y0; scanY <= y1; scanY++) {
        float cx1 = currentX1;
        float cx2 = currentX2;
        if (cx2 < cx1) std::swap(cx1, cx2);
        
        // Draw horizontal line
        for (int i = (int) cx1 + 1.0f; i <= (int) cx2; i++) {
            this->set_pixel(i, scanY, with);
        }
       
        currentX1 += inverseSlope1;
        currentX2 += inverseSlope2;
    }
}

void ic::Image::fill_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, const image_t &with) {
    float inverseSlope1 = (x2 - x0) / (float) (y2 - y0);
    float inverseSlope2 = (x2 - x1) / (float) (y2 - y1);

    float currentX1 = x2, currentX2 = x2;
    
    for (int scanY = y2; scanY > y0; scanY--) {
        float cx1 = currentX1;
        float cx2 = currentX2;
        if (cx2 < cx1) std::swap(cx1, cx2);
        
        // Draw horizontal line
        for (int i = (int) cx1 + 1.0f; i <= (int) cx2; i++) {
            this->set_pixel(i, scanY, with);
        }

        currentX1 -= inverseSlope1;
        currentX2 -= inverseSlope2;
    }
}

void ic::Image::blit(const image_t *data, int topLeftX, int topLeftY, int w, int h) {
    // Obtain clipped bounds with the main image
    int clippedTopLeftX = ic::Mathf::get().clamp(topLeftX, 0, this->width);
    int clippedTopLeftY = ic::Mathf::get().clamp(topLeftY, 0, this->height);

    int clippedBottomRightX = ic::Mathf::get().clamp(topLeftX + w, 0, this->width);
    int clippedBottomRightY = ic::Mathf::get().clamp(topLeftY + h, 0, this->height);

    for (int j = clippedTopLeftY - topLeftY; j < clippedBottomRightY - (topLeftY + h) + h; j++) {
        for (int i = clippedTopLeftX - topLeftX; i < clippedBottomRightX - (topLeftX + w) + w; i++) {
            this->set_pixel_unsafe(i + topLeftX, j + topLeftY, data[j * w + i]);
        }
    }
}

void ic::Image::blit(const ic::Image &image, int topLeftX, int topLeftY) {
    this->blit(image.pixels, topLeftX, topLeftY, image.width, image.height);
}

void ic::Image::blit_png(const std::string &fileName, int topLeftX, int topLeftY) {
    this->blit(ic::ImageIO::get().read_png(fileName), topLeftX, topLeftY);
}

void ic::Image::blit(const std::string &fileName, int topLeftX, int topLeftY) {
    this->blit_png(fileName, topLeftX, topLeftY);
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