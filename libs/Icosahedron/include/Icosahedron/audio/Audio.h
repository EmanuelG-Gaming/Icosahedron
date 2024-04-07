#ifndef IC_AUDIO_H
#define IC_AUDIO_H

#include <iostream>
#include <memory>
#include <vector>

#include "soloud.h"

#include <Icosahedron/audio/Music.h>
#include <Icosahedron/audio/Sound.h>
#include <Icosahedron/audio/AudioSource.h>
#include <Icosahedron/math/geom/Vectors.h>


namespace ic {
    namespace Audio {
        void init();
        void dispose();

        void play(ic::Music *music);
        void play(ic::Sound *sound);
        void play3D(ic::Sound *sound, float x, float y, float z, float vx, float vy, float vz, float volume, bool paused);
        void play3D(ic::Sound *sound, const ic::Vec3 &pos, const ic::Vec3 &vel, float volume, bool paused);

        void set_listener_position(float x, float y, float z);
        void set_listener_position(const ic::Vec3 &pos);

        void set_listener_velocity(float x, float y, float z);
        void set_listener_velocity(const ic::Vec3 &vel);

        void update();

        /** @brief Calculates the fast Fourier Transform of the entire audio pool. 
         *  The amount of "bars", or frequencies of the resulting array is 256.
        */
        float *calculate_FFT();
        float *calculate_waveform();
            
        namespace {
            std::unique_ptr<SoLoud::Soloud> handler;
            std::vector<ic::AudioSource> sources;

            bool initialized = false;
            bool disposed = false;
        };
    };
}
#endif