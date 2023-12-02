#include <Icosahedron/assets/loaders/TextureLoader.h>


ic::Texture ic::TextureLoader::load_png(const std::string &filePath, ic::TextureParameters parameters, bool gammaCorrection) {
    SDL_Surface *texture = IMG_Load(filePath.c_str());
    if (texture == NULL) {
        fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
        return ic::Texture();
    }

    GLenum textureFormat = this->map_to_texture_format(texture->format->format, gammaCorrection);
    ic::Texture result = this->load_texture(texture->pixels, texture->w, texture->h, textureFormat, textureFormat, parameters);
    SDL_FreeSurface(texture);

    return result;
}

ic::Texture ic::TextureLoader::load_bmp(const std::string &filePath, ic::TextureParameters parameters, bool gammaCorrection) {
    SDL_Surface *texture = SDL_LoadBMP(filePath.c_str());
    if (texture == NULL) {
        fprintf(stderr, "SDL_LoadBMP Error: %s\n", SDL_GetError());
        return ic::Texture();
    }

    GLenum textureFormat = this->map_to_texture_format(texture->format->format, gammaCorrection);
    ic::Texture result = this->load_texture(texture->pixels, texture->w, texture->h, textureFormat, textureFormat, parameters);
    SDL_FreeSurface(texture);

    return result;
}


ic::Texture ic::TextureLoader::load(ic::Image &image, ic::TextureParameters parameters, bool gammaCorrection) {
    GLenum format = gammaCorrection ? GL_SRGB : GL_RGB;
    return this->load_texture(image.data(), image.get_width(), image.get_height(), format, format, parameters);
}


GLenum ic::TextureLoader::map_to_texture_format(uint32_t format, bool gammaCorrection) {
    GLenum result = 0;

    switch (format) {
        case SDL_PIXELFORMAT_RGB24:
            result = gammaCorrection ? GL_SRGB : GL_RGB;
            break;

        case SDL_PIXELFORMAT_RGBA32:
            result = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            break;

        default:
            result = gammaCorrection ? GL_SRGB : GL_RGB;
            break;
    }
    
    return result;
}

ic::Texture ic::TextureLoader::load_texture(const void *data, int width, int height, GLenum internalFormat, GLenum format, const ic::TextureParameters &parameters) {
    return ic::Texture().setup_from_array(data, width, height, internalFormat, format, parameters);
}