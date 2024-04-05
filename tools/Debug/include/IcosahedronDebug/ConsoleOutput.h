#ifndef IC_DEBUG_CONSOLE_OUTPUT_H
#define IC_DEBUG_CONSOLE_OUTPUT_H

#include <stdio.h>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

namespace ic { namespace Debug {
    static void create_console() {
#ifdef _WIN32
        AllocConsole();
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
#endif
    }

    /** @brief By default, SDL2 prevents the command prompt from being shown on startup (at least on Windows) 
     *  and even halts the user from directly outputting text to the terminal (i.e. in real-time).
     *  Though, the text is still logged in to stdin/stdout/stderr, and even the fprintf() function takes in these file locations as parameters.
     * 
     *  This utility helps the user log the console to a file.
    */
    class ConsoleOutput {
        public:
            static ConsoleOutput& get() {
                static ConsoleOutput ins;
                return ins;
            }
    
            /** @brief Writes to a file, based on a stream.
             *  @param stream Can be either stdin. stdout or stderr, but it can also be a custom file.
             */
            void write_file(const std::string &fileName, FILE *stream);

        private:
            ConsoleOutput() {}
            ~ConsoleOutput() {}

        public:
            ConsoleOutput(ConsoleOutput const&) = delete;
            void operator = (ConsoleOutput const&) = delete;
    };
}}

#endif