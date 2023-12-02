#include <Icosahedron/graphics/gl/Cubemap.h>
#include <iostream>

using namespace ic;


Cubemap::Cubemap() {}

ic::Cubemap &ic::Cubemap::setup() {
    glGenTextures(1, &this->textureIndex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureIndex);

    return *this;
}


void ic::Cubemap::use(int index) {
    if (!this->textureIndex) {
        std::cerr << "Couldn't bind the cube map at index " << index << ". It wasn't set up." << "\n";
        return;
    }
    
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureIndex);
}

void ic::Cubemap::unuse() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void ic::Cubemap::dispose() {
    if (!this->textureIndex) {
        std::cerr << "Couldn't dispose the cube map. It wasn't loaded first." << "\n";
        return;
    }
       
    glDeleteTextures(1, &this->textureIndex);
}