#ifndef IC_SOUND_H
#define IC_SOUND_H

#include <string>

#include "soloud.h"
#include "soloud_wav.h"


namespace ic {
    class Sound {
        public:
            Sound(const std::string &filePath);

            SoLoud::Wav &get_stream();
        
        private:
            void load(const std::string &filePath);

        private:
            SoLoud::Wav stream;
    };
}
#endif