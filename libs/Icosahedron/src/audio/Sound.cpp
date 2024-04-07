#include <Icosahedron/audio/Sound.h>

using namespace ic;

Sound::Sound(const char *filePath) {
    this->load(filePath);
}

void ic::Sound::load(const char *filePath) {
    SoLoud::result result = this->stream.load(filePath);
    if (result == SoLoud::FILE_NOT_FOUND) printf("Couldn't load the sound. File was not found.\n");
    else if (result == SoLoud::FILE_LOAD_FAILED) printf("Couldn't load the sound. File loading failed. \n");
}

void ic::Sound::update() {
}


SoLoud::Wav &ic::Sound::get_stream() {
    return this->stream;
}