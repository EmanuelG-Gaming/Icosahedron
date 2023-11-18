#include <Icosahedron/graphics/gl/TextureArray.h>
#include <Icosahedron/Global.h>

using namespace ic;


TextureArray::TextureArray() {
    this->textureWidth = 8;
    this->textureHeight = 8;
    this->numberOfTextures = 1;
}

TextureArray::TextureArray(int textureWidth, int textureHeight, unsigned int numberOfTextures) {
    this->textureWidth = textureWidth;
    this->textureHeight = textureHeight;
    this->numberOfTextures = numberOfTextures;

    if (IC_IS_OPENGL_CONTEXT_PRESENT) {
        this->setup(this->textureWidth, this->textureHeight, this->numberOfTextures);
    }
}

TextureArray::TextureArray(int textureSize, unsigned int numberOfTextures) : TextureArray(textureSize, textureSize, numberOfTextures) {
}


void ic::TextureArray::add_texture(const std::string &fileName) {
    SDL_Surface *surface = IMG_Load(fileName.c_str());
    if (surface == NULL) {
        printf("Couldn't load the surface!\n");
        return;
    }

    if (this->texturesUsed >= this->numberOfTextures) {
        printf("Texture amount has been exceeded!\n");
        return;
    }


    glBindTexture(GL_TEXTURE_2D_ARRAY, this->textureIndex);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, this->texturesUsed, this->textureWidth, this->textureHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_LOD_BIAS, -1);

    this->texturesUsed++;

    SDL_FreeSurface(surface);
}

void ic::TextureArray::use(int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D_ARRAY, this->textureIndex);
}

void ic::TextureArray::unuse() {
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void ic::TextureArray::dispose() {
    glDeleteTextures(1, &this->textureIndex);
}

void ic::TextureArray::setup(int textureWidth, int textureHeight, unsigned int numberOfTextures) {
    glGenTextures(1, &this->textureIndex);
    glBindTexture(GL_TEXTURE_2D_ARRAY, this->textureIndex);

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, textureWidth, textureHeight, numberOfTextures);


    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}