#ifndef IC_DEBUG_H
#define IC_DEBUG_H

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif


namespace ic {
    /** @brief This makes a terminal that contains the standard input, output and errors from the application on Windows platforms. 
     *  This does not necesarily require you to set up a graphical application.
     *  Credits: https://wiki.libsdl.org/SDL2/FAQWindows.
    */
    void create_console();

    

    /** @brief Writes to a file, based on a stream.
     *  @param stream Can be either stdin. stdout or stderr, but it can also be a custom file.
    */
    void write_file(const char *fileName, FILE *stream);
};

#endif