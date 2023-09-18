#include <Icosahedron/audio/Music.h>
#include <Icosahedron/files/File.h>

using namespace ic;


Music::Music(const std::string &filePath) {
    this->load(filePath);
}

void ic::Music::load(const std::string &filePath) {
    this->stream.load(filePath.c_str());
    //if (!result) printf("Couldn't load the music.\n");
}

void ic::Music::play() {
    this->played = true;
}
void ic::Music::set_volume(float amount) {
    this->volume = amount;
}


SoLoud::WavStream &ic::Music::get_stream() {
    return this->stream;
}


bool &ic::Music::is_played() {
    return this->played;
}

float &ic::Music::get_volume() {
    return this->volume;
}