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
            Music(const std::string &filePath);

            void play();
            void set_volume(float amount);

            SoLoud::WavStream &get_handler();
            bool &is_played();
            float &get_volume();
            
        private:
            void load(const std::string &filePath);

        private:
            SoLoud::WavStream handler;
            bool played = false;
            float volume = 1.0f;
    };
}
#endif