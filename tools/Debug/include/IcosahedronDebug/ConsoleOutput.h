#ifndef IC_DEBUG_CONSOLE_OUTPUT_H
#define IC_DEBUG_CONSOLE_OUTPUT_H

#include <stdio.h>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

namespace ic {
    /** @brief By default, SDL2 prevents the command prompt from being shown on startup (at least on Windows) 
     *  and even halts the user from directly outputting text to the terminal (i.e. in real-time).
     *  Though, the text is still logged in to stdin/stdout/stderr, and even the fprintf() function takes in these file locations as parameters.
     * 
     *  This utility helps the user log the console to a file, and for Windows, this can also be used to create a separate terminal.
    */ 
    namespace Debug {
        /** @brief This makes a terminal that contains the standard input, output and errors from the application on Windows platforms. 
         *  This does not necesarily require an ic::Application instance.
         *  Credits: https://wiki.libsdl.org/SDL2/FAQWindows.
        */
        void create_console();

        

        /** @brief Writes to a file, based on a stream.
         *  @param stream Can be either stdin. stdout or stderr, but it can also be a custom file.
        */
        void write_file(const char *fileName, FILE *stream);
    }
}

#endif