#include <Icosahedron/assets/loaders/TextureLoader.h>


ic::Texture *ic::TextureLoader::load_png(const std::string &filePath, ic::TextureParameters parameters, bool gammaCorrection) {
    SDL_Surface *texture = IMG_Load(filePath.c_str());
    if (texture == NULL) {
        fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
        ic::Texture *result = new ic::Texture(ic::T2D);
        return result;
    }

    GLenum textureFormat = this->map_to_texture_format(texture->format->format, gammaCorrection);
    ic::Texture *result = this->load_texture(texture->w, texture->h, textureFormat, texture->pixels, parameters);
    SDL_FreeSurface(texture);

    return result;
}

ic::Texture *ic::TextureLoader::load_bmp(const std::string &filePath, ic::TextureParameters parameters, bool gammaCorrection) {
    SDL_Surface *texture = SDL_LoadBMP(filePath.c_str());
    if (texture == NULL) {
        fprintf(stderr, "SDL_LoadBMP Error: %s\n", SDL_GetError());
        ic::Texture *result = new ic::Texture(ic::T2D);
        return result;
    }

    GLenum textureFormat = this->map_to_texture_format(texture->format->format, gammaCorrection);
    ic::Texture *result = this->load_texture(texture->w, texture->h, textureFormat, texture->pixels, parameters);
    SDL_FreeSurface(texture);

    return result;
}

ic::Texture *ic::TextureLoader::load(ic::Image &image, ic::TextureParameters parameters, bool gammaCorrection) {
    return this->load_texture(image.get_width(), image.get_height(), gammaCorrection ? GL_SRGB : GL_RGB, image.data(), parameters);
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

ic::Texture *ic::TextureLoader::load_texture(int width, int height, GLenum format, const void *data, const ic::TextureParameters &parameters) {
    ic::Texture *result = new ic::Texture(ic::T2D);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    if (parameters.usesMipmapping && ic::Mathf::get().is_power_of(2, width)) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameters.wrapU);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameters.wrapV);
    if (!parameters.usesMipmapping) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameters.minFilter);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameters.minFilter == ic::TEXTURE_FILTER_NEAREST ? 
                                                     GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameters.magFilter);


    glBindTexture(GL_TEXTURE_2D, 0);

    return result;
}