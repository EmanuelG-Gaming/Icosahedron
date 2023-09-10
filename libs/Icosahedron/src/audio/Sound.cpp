#include <Icosahedron/audio/Sound.h>

using namespace ic;


Sound::Sound(const std::string &filePath) {
    this->handler = NULL;

    this->load(filePath);
}


void ic::Sound::play() {

}
void ic::Sound::dispose() {

}


void ic::Sound::load(const std::string &filePath) {
    auto string = filePath.c_str();
    ic::File file = ic::File(filePath);
    
    if (this->handler == NULL) {
        std::string error = std::string(Mix_GetError());
        throw std::runtime_error("Mix_LoadMUS Error: " + error + "\n");
    }
}