#ifndef IC_AUDIO_H
#define IC_AUDIO_H

#include <iostream>
#include <memory>
#include <vector>

#include "soloud.h"

#include <Icosahedron/audio/Music.h>


namespace ic {
    class Audio {
        public:
            static Audio& get() {
                static Audio ins;
                return ins;
            }

            void init();
            void dispose();

            void play(ic::Music *music);
            void playback();

            /** @brief Calculates the fast Fourier Transform of the entire audio pool. 
             *  The amount of "bars", or frequencies of the resulting array is 256.
            */
            float *calculate_FFT();
            float *calculate_waveform();
            
        private:
            Audio() {}
            ~Audio() {}

        public:
            Audio(Audio const&) = delete;
            void operator = (Audio const&) = delete;
        
        private:
            std::unique_ptr<SoLoud::Soloud> handler;
            std::vector<ic::Music*> music;
            
            bool initialized = false;
            bool disposed = false;
    };
}
#endif