#ifndef IC_MUSIC_H
#define IC_MUSIC_H

#include <string>

#include "soloud.h"
#include "soloud_wavstream.h"


namespace ic {
    /** @brief An audio instance that plays accross the entire scene.
    */
    class Music {
        public:
            Music(const char *filePath);

            SoLoud::WavStream &get_stream();
            
        private:
            void load(const char *filePath);

        private:
            SoLoud::WavStream stream;
    };
}
#endif