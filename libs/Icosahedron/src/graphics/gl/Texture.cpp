#include <Icosahedron/graphics/gl/Texture.h>

using namespace ic;


Texture::Texture(ic::GLTextureTypes type) {
    this->type = type;

    glGenTextures(1, &this->textureIndex);
    glBindTexture(this->type, this->textureIndex);
}

void ic::Texture::use(int index) {
    if (this->textureIndex) {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(this->type, this->textureIndex);
    }
}

void ic::Texture::unuse() {
    glBindTexture(this->type, 0);
}

void ic::Texture::dispose() {
    if (this->textureIndex) glDeleteTextures(1, &this->textureIndex);
}


ic::GLTextureTypes ic::Texture::get_type() {
    return this->type;
}