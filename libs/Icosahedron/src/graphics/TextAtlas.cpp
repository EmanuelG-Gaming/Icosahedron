#include <Icosahedron/graphics/TextAtlas.h>

using namespace ic;

TextAtlas::TextAtlas(FT_Face font) {
    this->font = font;
    this->glyph = font->glyph;
              
    this->adjust_size();
}

void TextAtlas::load() {
    this->add_empty_texture();
              
    int x = 0;
    for (int i = 32; i < 128; i++) {
        if (FT_Load_Char(font, i, FT_LOAD_RENDER)) {
            continue;
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, glyph->bitmap.width, glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
                   
        CharacterInfo ch;
        ch.advX = glyph->advance.x >> 6;
        ch.advY = glyph->advance.y >> 6;
                   
        ch.bitmapWidth = glyph->bitmap.width;
        ch.bitmapHeight = glyph->bitmap.rows;
                   
        ch.bitmapLeft = glyph->bitmap_left;
        ch.bitmapTop = glyph->bitmap_top;
                   
        ch.offsetX = (float) x / atlasWidth;
                   
        characters[i] = ch;
                   
        x += glyph->bitmap.width;
    }
}


void TextAtlas::use() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureIndex);
}
void TextAtlas::dispose() {
    glDeleteTextures(1, &textureIndex);
    FT_Done_Face(font);
}


void TextAtlas::adjust_size() {
    int width = 0;
    int height = 0;
              
    // Load ASCII characters
    for (int i = 32; i < 128; i++) {
        if (FT_Load_Char(font, i, FT_LOAD_RENDER)) {
            printf("Couldn't load character %c.\n", i);
            continue;
        }
                   
        width += glyph->bitmap.width;
        if (glyph->bitmap.rows > height) {
            height = glyph->bitmap.rows;
        }
    }
    this->atlasWidth = width;
    this->atlasHeight = height;
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