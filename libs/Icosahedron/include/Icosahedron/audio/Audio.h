#ifndef IC_AUDIO_H
#define IC_AUDIO_H

#include <iostream>
#include <memory>
#include <vector>

#include "soloud.h"
#include "soloud_audiosource.h"

#include <Icosahedron/audio/Music.h>
#include <Icosahedron/audio/Sound.h>
#include <Icosahedron/audio/AudioSource.h>
#include <Icosahedron/math/geom/Vectors.h>


namespace ic {
    enum SoundVolumeAttenuationModels {
        INVALID_ATTENUATION_MODEL = SoLoud::AudioSource::NO_ATTENUATION,
        NO_ATTENUATION_MODEL = SoLoud::AudioSource::NO_ATTENUATION,

        INVERSE_DISTANCE_ATTENUATION = SoLoud::AudioSource::INVERSE_DISTANCE,
        LINEAR_DISTANCE_ATTENUATION = SoLoud::AudioSource::LINEAR_DISTANCE,
        EXPONENTIAL_DISTANCE_ATTENUATION = SoLoud::AudioSource::EXPONENTIAL_DISTANCE,
    };

    namespace Audio {
        
        void init();
        void dispose();



        SoLoud::handle play(ic::Music *music, float volume = -1.0f, float panning = 0.0f, bool paused = false);
        SoLoud::handle play(ic::Sound *sound, float volume = -1.0f, float panning = 0.0f, bool paused = false);
        SoLoud::handle play3D(ic::Sound *sound, float x, float y, float z, float vx, float vy, float vz, float volume = 1.0f, bool paused = false);
        SoLoud::handle play3D(ic::Sound *sound, const ic::Vec3 &pos, const ic::Vec3 &vel, float volume = 1.0f, bool paused = false);

        void set_listener_position(float x, float y, float z);
        void set_listener_position(const ic::Vec3 &pos);

        void set_listener_velocity(float x, float y, float z);
        void set_listener_velocity(const ic::Vec3 &vel);

        void set_source_attenuation_params(SoLoud::handle &handle, float minDistance, float maxDistance, float attenuationRollofFactor, unsigned int attenuationModel);


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