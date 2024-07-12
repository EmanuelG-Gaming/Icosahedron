#include <Icosahedron/assets/loaders/TextureLoader.h>
#include <Icosahedron/graphics/ImageIO.h>


ic::Texture ic::TextureLoader::load(const char *filePath, ic::TextureParameters parameters, bool gammaCorrection) {
    int width, height, numberOfChannels;

    unsigned char *data = stbi_load(filePath, &width, &height, &numberOfChannels, 0);

    GLenum textureFormat = map_to_texture_format(numberOfChannels, gammaCorrection);
    ic::Texture result = load_texture(data, width, height, textureFormat, textureFormat, parameters);

    stbi_image_free(data);

    return result;
}

ic::Texture ic::TextureLoader::load(const std::string &filePath, ic::TextureParameters parameters, bool gammaCorrection) {
    return ic::TextureLoader::load(filePath.c_str(), parameters, gammaCorrection);
}




ic::Texture ic::TextureLoader::load(ic::Image &image, ic::TextureParameters parameters, bool gammaCorrection) {
    bool transparent = ic::ImageIO::image_transparent(image);
    GLenum format = gammaCorrection ? (transparent ? GL_SRGB_ALPHA : GL_SRGB) : (transparent ? GL_RGBA : GL_RGB);
    
    return load_texture(image.data(), image.get_width(), image.get_height(), format, format, parameters);
}


GLenum ic::TextureLoader::map_to_texture_format(int numberOfChannels, bool gammaCorrection) {
    GLenum result = 0;

    switch (numberOfChannels) {
        case 1:
            result = GL_RED;
            break;

        case 2:
            result = GL_RG;
            break;

        case 3:
            result = gammaCorrection ? GL_SRGB : GL_RGB;
            break;

        case 4:
            result = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            break;

        // By default, the format uses plain RGB
        default:
            result = gammaCorrection ? GL_SRGB : GL_RGB;
            break;
    }
    
    return result;
}

namespace ic::TextureLoader { namespace {
ic::Texture load_texture(const void *data, int width, int height, GLenum internalFormat, GLenum format, const ic::TextureParameters &parameters) {
    return ic::Texture().setup_from_array(data, width, height, internalFormat, format, parameters);
}
}}