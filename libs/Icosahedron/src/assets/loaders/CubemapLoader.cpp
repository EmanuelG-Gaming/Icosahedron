#include <Icosahedron/assets/loaders/CubemapLoader.h>

ic::Cubemap ic::CubemapLoader::load_png(const std::vector<std::string> &filePaths, bool gammaCorrection) {
    ic::Cubemap result;

    if (filePaths.size() != 6) {
        printf("Couldn't load the cubemap that has more or less faces than 6.\n");
        return result;
    }

    for (int i = 0; i < filePaths.size(); i++) {
        std::string filePath = filePaths[i];
        SDL_Surface *texture = IMG_Load(filePath.c_str());

        if (texture == NULL) {
            fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
            return result;
        }
    
        GLenum textureFormat = this->map_to_texture_format(texture->format->format, gammaCorrection);
        ic::CubemapFaceInformation face = { texture->w, texture->h, i, textureFormat, texture->pixels };

        this->load_cubemap_face(face);
        SDL_FreeSurface(texture);
    }

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    return result;
}

ic::Cubemap ic::CubemapLoader::load_bmp(const std::vector<std::string> &filePaths, bool gammaCorrection) {
    ic::Cubemap result;

    if (filePaths.size() != 6) {
        printf("Couldn't load cubemap faces with amount other than 6.\n");
        return result;
    }

    for (int i = 0; i < filePaths.size(); i++) {
        std::string filePath = filePaths[i];
        SDL_Surface *texture = SDL_LoadBMP(filePath.c_str());

        if (texture == NULL) {
            fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
            return result;
        }
    
        GLenum textureFormat = this->map_to_texture_format(texture->format->format, gammaCorrection);
        ic::CubemapFaceInformation face = { texture->w, texture->h, i, textureFormat, texture->pixels };

        this->load_cubemap_face(face);
        SDL_FreeSurface(texture);
    }

    return result;
}



GLenum ic::CubemapLoader::map_to_texture_format(uint32_t format, bool gammaCorrection) {
    GLenum result;

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

void ic::CubemapLoader::load_cubemap_face(const ic::CubemapFaceInformation &data) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + data.faceIndex, 0, data.format, data.width, data.height, 0, data.format, GL_UNSIGNED_BYTE, data.data);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
}
