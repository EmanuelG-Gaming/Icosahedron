#include <Icosahedron/audio/Audio.h>

void ic::Audio::init() {
    if (initialized) {
        std::cerr << "Couldn't initialize the audio handler more than once!" << "\n";
        return;
    }

    handler = std::make_unique<SoLoud::Soloud>();
    handler->init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);

    initialized = true;
}

void ic::Audio::dispose() {
    if (disposed) {
        std::cerr << "Couldn't dispose the audio handler more than once!" << "\n";
        return;
    }

    handler->deinit();

    disposed = true;
}

void ic::Audio::play(ic::Music *music) {
    handler->play(music->get_stream());
}
void ic::Audio::play(ic::Sound *sound) {
    handler->play(sound->get_stream());
}

float *ic::Audio::calculate_FFT() {
    return handler->calcFFT();
}

float *ic::Audio::calculate_waveform() {
    return handler->getWave();
}