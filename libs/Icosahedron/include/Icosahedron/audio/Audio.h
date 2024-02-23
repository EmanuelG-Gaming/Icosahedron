#ifndef IC_AUDIO_H
#define IC_AUDIO_H

#include <iostream>
#include <memory>
#include <vector>

#include "soloud.h"

#include <Icosahedron/audio/Music.h>
#include <Icosahedron/audio/Sound.h>


namespace ic {
    namespace Audio {
        void init();
        void dispose();

        void play(ic::Music *music);
        void play(ic::Sound *sound);
        
        /** @brief Calculates the fast Fourier Transform of the entire audio pool. 
         *  The amount of "bars", or frequencies of the resulting array is 256.
        */
        float *calculate_FFT();
        float *calculate_waveform();
            
        namespace {
            std::unique_ptr<SoLoud::Soloud> handler;
            
            bool initialized = false;
            bool disposed = false;
        };
    };
}
#endif