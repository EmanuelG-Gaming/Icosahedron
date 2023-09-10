#include <Icosahedron/audio/Music.h>

using namespace ic;


Music::Music(const std::string &filePath) {
    this->previousVolume = this->currentVolume = 0;
    this->handler = NULL;
    
    this->load(filePath);
}

void ic::Music::play(int loopCount) {
    this->currentVolume = 1;

    Mix_VolumeMusic(this->currentVolume);
    Mix_ResumeMusic();

    if (Mix_PlayMusic(this->handler, loopCount) == -1) {
        fprintf(stderr, "Couldn't play music.\n");
        return;
    }
}


void ic::Music::mute() {
    int v = 0;
    if (this->currentVolume) {
        v = Mix_VolumeMusic(-1);
    }

    if (v != 0) {
        this->previousVolume = v;
        this->currentVolume = 0;
    }

    Mix_VolumeMusic(0);
}
void ic::Music::unmute() {
    this->currentVolume = 1;
    Mix_VolumeMusic(this->currentVolume);
}
void ic::Music::pause() {
    Mix_PauseMusic();
}
void ic::Music::resume() {
    Mix_ResumeMusic();
}


void ic::Music::dispose() {
    Mix_FreeMusic(this->handler);
}


bool ic::Music::is_playing() {
    return (bool) Mix_PlayingMusic();
}
bool ic::Music::is_paused() {
    return (bool) Mix_PausedMusic();
}
bool ic::Music::is_muted() {
    return (this->currentVolume == 0);
}

int ic::Music::get_volume() {
    return this->currentVolume;
}


void ic::Music::load(const std::string &filePath) {
    this->handler = Mix_LoadMUS(filePath.c_str());
    if (this->handler == NULL) {
        std::string error = std::string(Mix_GetError());
        throw std::runtime_error("Mix_LoadMUS Error: " + error + "\n");
    }
}