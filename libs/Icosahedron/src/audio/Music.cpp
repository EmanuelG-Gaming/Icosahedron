#include <Icosahedron/audio/Music.h>

using namespace ic;


Music::Music(const std::string &filePath) {
    this->load(filePath);
}

void ic::Music::load(const std::string &filePath) {
    SoLoud::result result = this->stream.load(filePath.c_str());
    if (result == SoLoud::FILE_NOT_FOUND) printf("Couldn't load the music. File was not found.\n");
    else if (result == SoLoud::FILE_LOAD_FAILED) printf("Couldn't load the music.\n");
}

SoLoud::WavStream &ic::Music::get_stream() {
    return this->stream;
}