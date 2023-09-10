#ifndef IC_SOUND_H
#define IC_SOUND_H

#include <string>
#include <stdexcept>

#include <SDL2/SDL_mixer.h>

#include <Icosahedron/files/File.h>


namespace ic {
    class Sound {
        public:
            Sound(const std::string &filePath);

            void play();

            void dispose();

        private:
            void load(const std::string &filePath);

        private:
            Mix_Chunk *handler;
    };
}
#endif