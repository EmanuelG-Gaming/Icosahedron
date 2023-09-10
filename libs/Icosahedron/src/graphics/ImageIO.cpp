#include <Icosahedron/graphics/ImageIO.h>

ic::Image ic::ImageIO::read_png(ic::File file) {
    SDL_Surface *surface = IMG_Load(file.get_path().c_str());
    if (surface == NULL) {
        throw std::runtime_error("IMG_Load Error: " + std::string(IMG_GetError()) + "\n");
    }

    int width = surface->w, height = surface->h;
    auto format = surface->format;
    int bytes = surface->format->BytesPerPixel;
    int pitch = surface->pitch;

    ic::image_t *colors = new ic::image_t[width * height];


    SDL_LockSurface(surface);

    // Retrieve the color of each pixel
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            uint8_t *pixelPointer = (uint8_t*) surface->pixels + j * pitch + i * bytes;
            uint32_t pixel = *(uint32_t*) pixelPointer;

            ic::image_t color = { 0x00, 0x00, 0x00 };
            
            SDL_GetRGB(pixel, format, &color.r, &color.g, &color.b);
            colors[j * width + i] = color;
        }
    }

    SDL_UnlockSurface(surface);


    ic::Image result = ic::Image(width, height, colors);
    SDL_FreeSurface(surface);
    
    return result;
}

ic::Image ic::ImageIO::read_bmp(ic::File file) {
    SDL_Surface *surface = SDL_LoadBMP(file.get_path().c_str());
    if (surface == NULL) {
        throw std::runtime_error("IMG_Load Error: " + std::string(IMG_GetError()) + "\n");
    }

    int width = surface->w, height = surface->h;
    auto format = surface->format;
    int bytes = surface->format->BytesPerPixel;
    int pitch = surface->pitch;

    ic::image_t *colors = new ic::image_t[width * height];


    SDL_LockSurface(surface);

    // Retrieve the color of each pixel
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            uint8_t *pixelPointer = (uint8_t*) surface->pixels + j * pitch + i * bytes;
            uint32_t pixel = *(uint32_t*) pixelPointer;

            ic::image_t color = { 0x00, 0x00, 0x00 };
            
            SDL_GetRGB(pixel, format, &color.r, &color.g, &color.b);
            colors[j * width + i] = color;
        }
    }

    SDL_UnlockSurface(surface);


    ic::Image result = ic::Image(width, height, colors);
    SDL_FreeSurface(surface);
    
    return result;
}

ic::Image ic::ImageIO::read_ppm(ic::File file) {
    std::ifstream read(file.get_path());

    if (!read.is_open() || read.fail()) {
        printf("Couldn't open the .ppm file, returning a default image instead.\n");
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
            // Reading from a pixel
            ic::Color pixel;
            stream >> pixel.r >> pixel.g >> pixel.b;

            data[i] = pixel;
            i++;
        }
    }

    ic::Image result = ic::Image(width, height, data);

    return result;
}



void ic::ImageIO::write_png(ic::File file, ic::Image &image) {
    SDL_Surface *surface = this->to_surface(image);
    if (surface == NULL) {
        return;
    }
    if (IMG_SavePNG(surface, file.get_path().c_str()) == -1) {
        printf("PNG image couldn't be saved!\n");
    }
    SDL_FreeSurface(surface);
}

void ic::ImageIO::write_bmp(ic::File file, ic::Image &image) {
    SDL_Surface *surface = this->to_surface(image);
    if (surface == NULL) {
        return;
    }
    if (SDL_SaveBMP(surface, file.get_path().c_str()) == -1) {
        printf("Bitmap image couldn't be saved!\n");
    }
    SDL_FreeSurface(surface);
}

void ic::ImageIO::write_ppm(ic::File file, ic::Image &image) {
    std::ofstream output(file.get_path());
    float width = image.get_width();
    float height = image.get_height();

    // "P3" means that this image is stored in RGB ASCII color space
    output << "P3" << "\n";
    output << width << " " << height << "\n";
    output << "255" << "\n";

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            ic::Color pixel = image.pixel_at(i, j);
            output << std::to_string(pixel.r) << " " << std::to_string(pixel.g) << " " << std::to_string(pixel.b) << "\n";
        }
    }

    output.close();
}


SDL_Surface *ic::ImageIO::to_surface(ic::Image &image) {
    int bytes = sizeof(uint8_t);
    
    int width = image.get_width();
    int height = image.get_height();
    int pitch = width * 3;
    int depth = 24;

    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom(image.data(), width, height, depth, pitch, SDL_PIXELFORMAT_RGB24);
    if (surface == NULL) {
        printf("SDL_CreateRGBSurfaceWithFormatFrom Error: %s\n", SDL_GetError());
        return NULL;
    }

    return surface;
}