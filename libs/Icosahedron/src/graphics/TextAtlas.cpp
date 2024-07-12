#include <Icosahedron/graphics/TextAtlas.h>
#include <Icosahedron/Global.h>

#include <iostream>


using namespace ic;


TextAtlas::TextAtlas() {
}


void TextAtlas::use() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureIndex);
}

void TextAtlas::dispose() {
    glDeleteTextures(1, &textureIndex);
}



void TextAtlas::add_empty_texture() {
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureIndex);
              
    glBindTexture(GL_TEXTURE_2D, textureIndex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
              
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
              
              
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}



std::array<CharacterInfo, 128> &TextAtlas::get_characters() { 
    return characters; 
}


float TextAtlas::get_width() { 
    return atlasWidth; 
}
float TextAtlas::get_height() { 
    return atlasHeight; 
}
