#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/math/Mathf.h>

using namespace ic;


Texture::Texture() {}

ic::Texture &ic::Texture::setup() {
    glGenTextures(1, &this->textureIndex);
    glBindTexture(GL_TEXTURE_2D, this->textureIndex);

    return *this;
}

void ic::Texture::use(int index) {
    if (!this->textureIndex) {
        std::cerr << "Couldn't bind the texture at index " << index << ". It wasn't set up." << "\n";
        return;
    }
    
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, this->textureIndex);
}

void ic::Texture::unuse() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ic::Texture::dispose() {
    if (!this->textureIndex) {
        std::cerr << "Couldn't dispose the texture. It wasn't loaded first." << "\n";
        return;
    }
       
    glDeleteTextures(1, &this->textureIndex);
}



ic::Texture &ic::Texture::set_pixel_content(const void *content, GLenum format) {
    if (!this->textureIndex) {
        std::cerr << "Couldn't change the texture's pixels. It wasn't loaded first." << "\n";
        return *this;
    }

    ic::Vec2i size = this->get_dimensions();

    glBindTexture(GL_TEXTURE_2D, this->textureIndex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.x(), size.y(), format, GL_UNSIGNED_BYTE, content);

    return *this;
}


ic::Texture &ic::Texture::setup_from_array(const void *data, int width, int height, GLenum internalFormat, GLenum format, const ic::TextureParameters &parameters) {
    this->setup();

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    if (parameters.usesMipmapping && ic::Mathf::is_power_of(2, width) && ic::Mathf::is_power_of(2, height)) {
        glGenerateMipmap(GL_TEXTURE_2D);
        //std::cout << "Power-of-two texture found." << "\n";
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

    return *this;
}


ic::Vec2i ic::Texture::get_dimensions() {
    ic::Vec2i result;

    glBindTexture(GL_TEXTURE_2D, this->textureIndex);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &result.x());
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &result.y());

    return result;
}