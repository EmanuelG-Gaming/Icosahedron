#include <Icosahedron/graphics/gl/TextureAtlas.h>

#include <stb_image.h>

using namespace ic;

TextureAtlas::TextureAtlas(int width, int height) {
    this->lastX = 0;
    this->lastY = 0;
    this->lastRowHeight = 0;

    this->atlasWidth = width;
    this->atlasHeight = height;

    this->initialize();
}


TextureAtlas::TextureAtlas() {
    this->lastX = 0;
    this->lastY = 0;
    this->lastRowHeight = 0;

    this->atlasWidth = 8;
    this->atlasHeight = 8;
}

void ic::TextureAtlas::use() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureIndex);
}

void ic::TextureAtlas::unuse() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ic::TextureAtlas::dispose() {
    glDeleteTextures(1, &textureIndex);
}


ic::AtlasEntry ic::TextureAtlas::add_entry(const char *location, int w, int h, GLenum format, const void *pixels) {
    int width = w;
    int height = h;

    AtlasEntry entry;
    entry.x = this->lastX;
    entry.y = this->lastY;
    entry.width = width;
    entry.height = height;

    // Move to next row
    if (entry.x + entry.width > this->atlasWidth) {
        entry.x = 0;
        entry.y += this->lastRowHeight;
    
        this->lastRowHeight = 0;
        this->lastX = entry.x;
        this->lastY = entry.y;
    }

    // Calculate row height
    if (entry.height > this->lastRowHeight) {
        this->lastRowHeight = entry.height;
    }

    // If entry exceeded the maximum height...
    if (entry.x + entry.height > this->atlasHeight) {
        printf("Couldn't add texture. It exceeded the maximum atlas height.\n");
        return AtlasEntry();
    }

    this->lastX += entry.width;
    entry.u = entry.x / (float) this->atlasWidth;
    entry.v = entry.y / (float) this->atlasHeight;
    entry.u2 = (entry.x + entry.width) / (float) this->atlasWidth;
    entry.v2 = (entry.y + entry.height) / (float) this->atlasHeight;

    glTexSubImage2D(GL_TEXTURE_2D, 0, entry.x, entry.y, entry.width, entry.height, format, GL_UNSIGNED_BYTE, pixels);

    entries[location] = entry;

    // Finally, free pixels from heap
    //stbi_image_free(pixels);

    return entry;
}

ic::AtlasEntry ic::TextureAtlas::add_entry(const char *location, ic::Image &image) {
    return this->add_entry(location, image.get_width(), image.get_height(), (ic::ImageIO::image_transparent(image) ? GL_RGBA : GL_RGB), image.data());
}
ic::AtlasEntry ic::TextureAtlas::add_entry(const std::string &location, const std::string fileName) {
    return add_entry(location.c_str(), fileName.c_str());
}


ic::AtlasEntry ic::TextureAtlas::add_entry(const char *location, const char *fileName) {
    int width, height, numberOfChannels;

    unsigned char *data = stbi_load(fileName, &width, &height, &numberOfChannels, 0);

    GLenum textureFormat = ic::TextureLoader::map_to_texture_format(numberOfChannels, false);

    return this->add_entry(
        location, width, height, 
        textureFormat, 
        data
    );
}



void ic::TextureAtlas::add_entries(const std::vector<std::string> &from) {
    for (int i = 0; i < from.size(); i += 2) {
        std::string alias = from.at(i);
        std::string fileName = from.at(i + 1);
    
        add_entry(alias, fileName);
    }
}


AtlasEntry& ic::TextureAtlas::get_entry(const std::string &location) {
    return entries[location];
}

AtlasEntry& ic::TextureAtlas::get_entry(int index) {
    auto iterator = this->entries.begin();
    std::advance(iterator, index);

    return iterator->second;
}

float ic::TextureAtlas::get_width() { return atlasWidth; }
float ic::TextureAtlas::get_height() { return atlasHeight; }


void ic::TextureAtlas::initialize() {
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureIndex);

    glBindTexture(GL_TEXTURE_2D, textureIndex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}