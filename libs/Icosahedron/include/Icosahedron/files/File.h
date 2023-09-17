#ifndef IC_FILE_H
#define IC_FILE_H

#include <string>
#include <filesystem>
#include <algorithm>

#include <SDL2/SDL.h>


namespace ic {
    /** @brief Represents a file in memory, followed by its directory name. */
    class File {
        public:
            /** @brief Constructs a new file instance, followed by its relative path that will be concatenated
             *  to its absolute path. 
             *  @param relativePath the relative path, with slashes as separators.
            */
            File(const std::string &relativePath);

            /** @returns The full path, without any backslashes. */
            std::string get_path();

            /** @returns The file extension's name, beginning with ".". */
            std::string get_extension();

        private:
            //std::string relativePath, absolutePath;
            std::string fullPath;

            std::filesystem::path handler;
    };
}

#endif