#include <Icosahedron/graphics/Image.h>
#include <Icosahedron/graphics/ImageIO.h>

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/Mathf.h>

using namespace ic;

Image::Image() {
    this->width = 1;
    this->height = 1;
    this->pixels = new image_t[width * height];

    this->pixels[0] = ic::Colors::white;
}

Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    this->pixels = new image_t[width * height];

    for (int i = 0; i < this->width * this->height; i++) {
        this->pixels[i] = ic::Colors::white;
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


image_t ic::Image::pixel_at_unsafe(int x, int y) const {
    return this->pixels[y * width + x];
}
image_t ic::Image::pixel_at_unsafe(int index) const {
    return this->pixels[index];
}
image_t ic::Image::pixel_at(int x, int y) const {
    if (!this->inside(x, y)) return { 0, 0, 0 };
    return this->pixel_at_unsafe(x, y);
}
image_t ic::Image::pixel_at(int index) const {
    if (index < 0 || index >= width * height) return { 0, 0, 0 };
    return this->pixel_at_unsafe(index);
}


image_t &ic::Image::pixel_at_unsafe(int x, int y) {
    return this->pixels[y * width + x];
}
image_t &ic::Image::pixel_at_unsafe(int index) {
    return this->pixels[index];
}


void ic::Image::fill_circle(int x, int y, int radius, const image_t &with) {
    int radiusSquared = radius * radius;

    // Obtain clipped bounds with the main image,
    int clippedTopLeftX = ic::Mathf::clamp(x - radius, 0, this->width);
    int clippedTopLeftY = ic::Mathf::clamp(y - radius, 0, this->height);

    int clippedBottomRightX = ic::Mathf::clamp(x + radius, 0, this->width);
    int clippedBottomRightY = ic::Mathf::clamp(y + radius, 0, this->height);

    ic::Vec2i clipTopLeft = { clippedTopLeftX - x, clippedTopLeftY - y };
    ic::Vec2i clipBottomRight = { clippedBottomRightX - (x + radius) + radius, clippedBottomRightY - (y + radius) + radius };


    for (int j = clipTopLeft.y(); j < clipBottomRight.y(); j++) {
        int py = y + j;

        for (int i = clipTopLeft.x(); i < clipBottomRight.x(); i++) {
            int px = x + i;

            if (i*i + j*j <= radiusSquared) {
                this->pixel_at_unsafe(px, py) = with;
            }
        }
    }
}


///// Lines /////

void ic::Image::draw_line(int x0, int y0, int x1, int y1, const image_t &with) {
    ic::Rasterization::line(x0, y0, x1, y1, [&](int x, int y) {
        this->pixel_at(x, y) = with;
    });
}


///// Triangles /////

void ic::Image::line_triangle(int x0, int y0, int x1, int y1, int x2, int y2, const image_t &with) {
    this->draw_line(x0, y0, x1, y1, with);
    this->draw_line(x1, y1, x2, y2, with);
    this->draw_line(x2, y2, x0, y0, with);
}

void ic::Image::fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2, const image_t &with) {
    ic::Rasterization::triangle(x0, y0, x1, y1, x2, y2, [&](int x, int y) {
        this->pixel_at(x, y) = with;
    });
}

void ic::Image::line_quad(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, const image_t &with) {
    this->draw_line(x0, y0, x1, y1, with);
    this->draw_line(x1, y1, x2, y2, with);
    this->draw_line(x2, y2, x3, y3, with);
    this->draw_line(x3, y3, x0, y0, with);
}

void ic::Image::fill_quad(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, const image_t &with) {
    this->fill_triangle(x0, y0, x1, y1, x2, y2, with);
    this->fill_triangle(x0, y0, x2, y2, x3, y3, with);
}


///// Rectangles /////

void ic::Image::line_rectangle(int topLeftX, int topLeftY, int w, int h, const image_t &with) {
    this->draw_line(topLeftX, topLeftY, topLeftX + w, topLeftY, with);
    this->draw_line(topLeftX, topLeftY + h, topLeftX + w, topLeftY + h, with);

    this->draw_line(topLeftX, topLeftY, topLeftX, topLeftY + h, with);
    this->draw_line(topLeftX + w, topLeftY, topLeftX + w, topLeftY + h, with);
}

void ic::Image::fill_rectangle(int topLeftX, int topLeftY, int w, int h, const image_t &with) {
    // Obtain clipped bounds with the main image
    int clippedTopLeftX = ic::Mathf::clamp(topLeftX, 0, this->width);
    int clippedTopLeftY = ic::Mathf::clamp(topLeftY, 0, this->height);

    int clippedBottomRightX = ic::Mathf::clamp(topLeftX + w, 0, this->width);
    int clippedBottomRightY = ic::Mathf::clamp(topLeftY + h, 0, this->height);

    ic::Vec2i clipTopLeft = { clippedTopLeftX - topLeftX, clippedTopLeftY - topLeftY };
    ic::Vec2i clipBottomRight = { clippedBottomRightX - (topLeftX + w) + w, clippedBottomRightY - (topLeftY + h) + h };


    for (int j = clipTopLeft.y(); j < clipBottomRight.y(); j++) {
        for (int i = clipTopLeft.x(); i < clipBottomRight.x(); i++) {
            this->pixel_at_unsafe(i + topLeftX, j + topLeftY) = with;
        }
    }
}



///// Textures /////

void ic::Image::blit(const image_t *data, int topLeftX, int topLeftY, int w, int h) {
    // Obtain clipped bounds with the main image,
    // so that we're not checking on every single pixel of the blitted image for an intersection
    int clippedTopLeftX = ic::Mathf::clamp(topLeftX, 0, this->width);
    int clippedTopLeftY = ic::Mathf::clamp(topLeftY, 0, this->height);

    int clippedBottomRightX = ic::Mathf::clamp(topLeftX + w, 0, this->width);
    int clippedBottomRightY = ic::Mathf::clamp(topLeftY + h, 0, this->height);

    ic::Vec2i clipTopLeft = { clippedTopLeftX - topLeftX, clippedTopLeftY - topLeftY };
    ic::Vec2i clipBottomRight = { clippedBottomRightX - (topLeftX + w) + w, clippedBottomRightY - (topLeftY + h) + h };


    for (int j = clipTopLeft.y(); j < clipBottomRight.y(); j++) {
        for (int i = clipTopLeft.x(); i < clipBottomRight.x(); i++) {
            this->pixel_at_unsafe(i + topLeftX, j + topLeftY) = data[j * w + i];
        }
    }
}

void ic::Image::blit(const ic::Image &image, int topLeftX, int topLeftY) {
    this->blit(image.pixels, topLeftX, topLeftY, image.width, image.height);
}

void ic::Image::blit_png(const std::string &fileName, int topLeftX, int topLeftY) {
    this->blit(ic::ImageIO::read_png(fileName), topLeftX, topLeftY);
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
        this->pixel_at_unsafe(x, y) = tmpBuffer[y * this->width + x];
    });

    delete[] tmpBuffer;
}

int ic::Image::get_width() const {
    return this->width;
}

int ic::Image::get_height() const {
    return this->height;
}

bool ic::Image::inside(int x, int y) const {
    return (x >= 0 && y >= 0 && x < this->width && y < this->height);
}


void *ic::Image::data() const {
    return this->pixels;
}