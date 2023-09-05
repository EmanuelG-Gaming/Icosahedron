#include <Icosahedron/assets/loaders/TextureLoader.h>


ic::Texture *ic::TextureLoader::load(const std::string &filePath, ic::TextureParameters parameters) {
    ic::Texture *result = new ic::Texture(ic::T2D);

    SDL_Surface *texture = this->load_surface(filePath);
    if (texture == NULL) {
        printf("2D Texture couldn't load.\n");
        return result;
    }

    GLenum textureFormat = this->map_to_texture_format(texture->format->format);
    glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, texture->w, texture->h, 0, textureFormat, GL_UNSIGNED_BYTE, texture->pixels);
    SDL_FreeSurface(texture);


    if (parameters.usesMipmapping && ic::Mathf::get().is_power_of(2, texture->w)) {
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

ic::Texture *ic::TextureLoader::load(ic::Image &image, ic::TextureParameters parameters) {
    ic::Texture *result = new ic::Texture(ic::T2D);

    GLenum textureFormat = GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, image.get_width(), image.get_height(), 0, textureFormat, GL_UNSIGNED_BYTE, image.data());
    if (parameters.usesMipmapping && ic::Mathf::get().is_power_of(2, image.get_width())) {
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

SDL_Surface *ic::TextureLoader::load_surface(const std::string &path) {
    SDL_Surface *image = IMG_Load(path.c_str());
    if (image == NULL) {
        fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
        return NULL;
    }
    return image;
}

GLenum ic::TextureLoader::map_to_texture_format(uint32_t format) {
    GLenum result;

    switch (format) {
        case SDL_PIXELFORMAT_RGB24:
            result = GL_RGB;
            printf("RGB\n");
            break;

        case SDL_PIXELFORMAT_RGBA32:
            result = GL_RGBA;
            printf("RGBA\n");
            break;
        default:
            result = GL_RGB;
            printf("Still rgb?\n");
            break;
    }
    
    return result;
}
