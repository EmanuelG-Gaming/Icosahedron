#include <Icosahedron/graphics/TextAtlas.h>

#include <iostream>


using namespace ic;


TextAtlas::TextAtlas() {
    this->atlasWidth = 4;
    this->atlasHeight = 4;
}


void TextAtlas::use() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureIndex);
}

void TextAtlas::dispose() {
    glDeleteTextures(1, &textureIndex);
}

void TextAtlas::blit_glyph(float u1, float v1, float width, float height, const void *pixels) {
    glTexSubImage2D(GL_TEXTURE_2D, 0, u1, v1, width, height, GL_RED, GL_UNSIGNED_BYTE, pixels);
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

void TextAtlas::initialize_texture(const void *bitmap) {
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureIndex);
              
    glBindTexture(GL_TEXTURE_2D, textureIndex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
              
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
              
              
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}





float TextAtlas::get_width() { 
    return atlasWidth; 
}
float TextAtlas::get_height() { 
    return atlasHeight; 
}

ic::CharacterInfo &TextAtlas::glyph_at(int index) {
    return this->characters[index];
}