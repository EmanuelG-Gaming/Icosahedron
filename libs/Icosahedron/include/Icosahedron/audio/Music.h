#ifndef IC_MUSIC_H
#define IC_MUSIC_H

#include <string>
#include <stdexcept>

#include <SDL2/SDL_mixer.h>


namespace ic {
    /** @brief An audio instance that plays accross the entire scene.
    */
    class Music {
        public:
            Music(const std::string &filePath);

            /** @brief Plays this music, while also overriding the playback of other songs if present. If the music is muted or paused, it will play yet again
             *  at a volume.  
             *  @param loopCount How many times should this music be played. If this is 1 or 0, the music will only play once.
            */
            void play(int loopCount = 0);
            void mute();
            void unmute();
            void pause();
            void resume();
            
            void dispose();


            bool is_playing();
            bool is_paused();
            bool is_muted();

            int get_volume();
            
        private:
            void load(const std::string &filePath);

        private:
            Mix_Music *handler;
            int previousVolume;
            int currentVolume;
    };
}
#endif