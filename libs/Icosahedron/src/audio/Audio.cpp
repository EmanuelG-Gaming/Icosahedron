#include <Icosahedron/audio/Audio.h>

void ic::Audio::init() {
    if (initialized) {
        std::cerr << "Couldn't initialize the audio handler more than once!" << "\n";
        return;
    }

    handler = std::make_unique<SoLoud::Soloud>();

    // By default, SoLoud initializes with OpenGL's right-handed coordinate system
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


void ic::Audio::update() {
    handler->update3dAudio();
}

void ic::Audio::play(ic::Music *music) {
    handler->play(music->get_stream());
}
void ic::Audio::play(ic::Sound *sound) {
    handler->play(sound->get_stream());
}

void ic::Audio::play3D(ic::Sound *sound, float x, float y, float z, float vx, float vy, float vz, float volume, bool paused) {
    sound->spatial = true;
    handler->play3d(sound->get_stream(), x, y, z, vx, vy, vz, volume, paused, 0);
}
void ic::Audio::play3D(ic::Sound *sound, const ic::Vec3 &pos, const ic::Vec3 &vel, float volume, bool paused) {
    play3D(sound, pos.x(), pos.y(), pos.z(), vel.x(), vel.y(), vel.z(), volume, paused);
}


float *ic::Audio::calculate_FFT() {
    return handler->calcFFT();
}

float *ic::Audio::calculate_waveform() {
    return handler->getWave();
}



void ic::Audio::set_listener_position(float x, float y, float z) {
    handler->set3dListenerPosition(x, y, z);
}
void ic::Audio::set_listener_velocity(float x, float y, float z) {
    handler->set3dListenerVelocity(x, y, z);
}

void ic::Audio::set_listener_position(const ic::Vec3 &pos) {
    handler->set3dListenerPosition(pos.x(), pos.y(), pos.z());
}
void ic::Audio::set_listener_velocity(const ic::Vec3 &vel) {
    handler->set3dListenerVelocity(vel.x(), vel.y(), vel.z());
}