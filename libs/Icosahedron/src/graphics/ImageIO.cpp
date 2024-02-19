#include <Icosahedron/graphics/ImageIO.h>

ic::Image ic::ImageIO::read_png(const std::string &fileName) {
    SDL_Surface *surface = IMG_Load(fileName.c_str());
    if (surface == NULL) {
        throw std::runtime_error("IMG_Load Error: " + std::string(IMG_GetError()) + "\n");
    }

    ic::Image result = this->to_image(surface);
    SDL_FreeSurface(surface);
    
    return result;
}

ic::Image ic::ImageIO::read_bmp(const std::string &fileName) {
    SDL_Surface *surface = SDL_LoadBMP(fileName.c_str());
    if (surface == NULL) {
        throw std::runtime_error("SDL_LoadBMP Error: " + std::string(SDL_GetError()) + "\n");
    }

    ic::Image result = this->to_image(surface);
    SDL_FreeSurface(surface);
    
    return result;
}

ic::Image ic::ImageIO::read_ppm(const std::string &fileName) {
    std::ifstream read(fileName);

    if (!read.is_open() || read.fail()) {
        printf("Couldn't open the .ppm file, returning a default 1x1 image instead.\n");
        return ic::Image(1, 1);
    }

    int i = 0;
    int width = 0, height = 0;
    ic::image_t *data;
    std::string line;

    while (std::getline(read, line)) {
        if (!line.compare("") || !line.compare(" ")) {
            continue;
        }
    
        std::istringstream stream(line);
        std::string tmp;
        int wordCount = 0;
        while (stream >> tmp) { wordCount++; }

        if (wordCount == 2) {
            // Image size
            stream >> width >> height;
            data = new ic::image_t[width * height];
        } 
        else if (wordCount == 3) {
            // Reading from an RGB pixel
            ic::Color pixel;
            stream >> pixel.r >> pixel.g >> pixel.b;
            pixel.a = 0;

            data[i] = pixel;
            i++;
        }
        else if (wordCount == 4) {
            // Reading from an RGBA pixel
            ic::Color pixel;
            stream >> pixel.r >> pixel.g >> pixel.b >> pixel.a;
            
            data[i] = pixel;
            i++;
        }
    }

    ic::Image result = ic::Image(width, height, data);

    return result;
}



void ic::ImageIO::write_png(const std::string &fileName, ic::Image &image) {
    SDL_Surface *surface = this->to_surface(image);
    if (surface == NULL) {
        return;
    }
    if (IMG_SavePNG(surface, fileName.c_str()) == -1) {
        printf("PNG image couldn't be saved!\n");
    }
    SDL_FreeSurface(surface);
}

void ic::ImageIO::write_bmp(const std::string &fileName, ic::Image &image) {
    SDL_Surface *surface = this->to_surface(image);
    if (surface == NULL) {
        return;
    }
    if (SDL_SaveBMP(surface, fileName.c_str()) == -1) {
        printf("Bitmap image couldn't be saved!\n");
    }
    SDL_FreeSurface(surface);
}

void ic::ImageIO::write_ppm(const std::string &fileName, ic::Image &image) {
    std::ofstream output(fileName);
    float width = image.get_width();
    float height = image.get_height();

    bool transparent = this->image_transparent(image);

    if (transparent) {
        output << "P4" << "\n";
    } else {
        // "P3" means that this image is stored in RGB ASCII color space
        output << "P3" << "\n";
    }

    output << width << " " << height << "\n";
    output << "255" << "\n";

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            ic::Color pixel = image.pixel_at(i, j);

            if (transparent) {
                output << std::to_string(pixel.r) << " " << std::to_string(pixel.g) << " " << std::to_string(pixel.b) << " " << std::to_string(pixel.a) << " \n";
            } else {
                output << std::to_string(pixel.r) << " " << std::to_string(pixel.g) << " " << std::to_string(pixel.b) << "\n";
            }
        }
    }

    output.close();
}


SDL_Surface *ic::ImageIO::to_surface(ic::Image &image) {
    bool transparent = this->image_transparent(image);

    int width = image.get_width();
    int height = image.get_height();
    int pitch = width;
    pitch *= (transparent ? 4 : 3);
    int depth = (transparent ? 32 : 24);
    SDL_PixelFormatEnum formatEnum = (transparent ? SDL_PIXELFORMAT_RGBA32 : SDL_PIXELFORMAT_RGB24);

    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom(image.data(), width, height, depth, pitch, formatEnum);
    if (surface == NULL) {
        printf("SDL_CreateRGBSurfaceWithFormatFrom Error: %s\n", SDL_GetError());
        return NULL;
    }

    return surface;
}

ic::Image ic::ImageIO::to_image(SDL_Surface *surface) {
    int width = surface->w, height = surface->h;
    auto format = surface->format;
    int bytes = surface->format->BytesPerPixel; // 3 for RGB24 and 4 for RGBA32
    int pitch = surface->pitch;

    ic::image_t *colors = new ic::image_t[width * height];

    // Actual processing
    SDL_LockSurface(surface);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            uint8_t *pixelPointer = (uint8_t*) surface->pixels + j * pitch + i * bytes;
            uint32_t pixel = *(uint32_t*) pixelPointer;

            ic::image_t color;
            SDL_GetRGBA(pixel, format, &color.r, &color.g, &color.b, &color.a);

            colors[j * width + i] = color;
        }
    }

    SDL_UnlockSurface(surface);
    // -----------------------

    return ic::Image(width, height, colors);
}



bool ic::ImageIO::image_transparent(ic::Image &source) {
    int area = source.get_width() * source.get_height();

    for (int i = 0; i < area; i++) {
        if (source.pixel_at_unsafe(i).a > 0x00) {
            return true;
        }
    }

    return false;
}