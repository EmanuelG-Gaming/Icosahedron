#include <Icosahedron/audio/Audio.h>

void ic::Audio::init() {
    if (this->initialized) {
        std::cerr << "Couldn't initialize the audio handler more than once!" << "\n";
        return;
    }

    this->handler = std::make_unique<SoLoud::Soloud>();
    this->handler->init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);

    this->initialized = true;
}

void ic::Audio::dispose() {
    if (this->disposed) {
        std::cerr << "Couldn't dispose the audio handler more than once!" << "\n";
        return;
    }

    this->handler->deinit();

    this->disposed = true;
}

void ic::Audio::play(ic::Music *music) {
    this->music.emplace_back(music);
}

void ic::Audio::playback() {
    for (auto &element : this->music) {
        if (!element->is_played()) {
            auto stream = element->get_stream();

            stream.setVolume(element->get_volume());
            this->handler->play(stream);

            element->play();
        }
    }
}


float *ic::Audio::calculate_FFT() {
    return this->handler->calcFFT();
}

float *ic::Audio::calculate_waveform() {
    return this->handler->getWave();
}