#ifndef IC_SOUND_H
#define IC_SOUND_H

#include <string>

#include "soloud.h"
#include "soloud_wav.h"

#include <Icosahedron/audio/AudioSource.h>


namespace ic {
    class Sound : public AudioSource {
        public:
            bool spatial = false;
            
            Sound(const char *filePath);

            void update() override;

            SoLoud::Wav &get_stream();
        
        private:
            void load(const char *filePath);

        private:
            SoLoud::Wav stream;
    };
}
#endif