#include <Icosahedron/audio/Sound.h>

using namespace ic;

Sound::Sound(const std::string &filePath) {
    this->load(filePath);
}

void ic::Sound::load(const std::string &filePath) {
    SoLoud::result result = this->stream.load(filePath.c_str());
    if (result == SoLoud::FILE_NOT_FOUND) printf("Couldn't load the sound. File was not found.\n");
    else if (result == SoLoud::FILE_LOAD_FAILED) printf("Couldn't load the sound.\n");
}


SoLoud::Wav &ic::Sound::get_stream() {
    return this->stream;
}