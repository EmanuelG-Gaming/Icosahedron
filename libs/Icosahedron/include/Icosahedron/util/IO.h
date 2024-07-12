#ifndef IC_IO_H
#define IC_IO_H


#include <fstream>
#include <sstream>
#include <string>
#include <list>

namespace ic {
    namespace IO {
        /** @brief A framework that uses a finite state machine with std::(i/o)fstream for easier function notation. */
        namespace {
            std::ifstream readHandler;
            std::ofstream writeHandler;
        }
        
        
        void open(const char *fileName, std::ios::openmode writeMode = std::ofstream::out);
        void close();

        void read(const void *p, int size = 1);
        void write(const void *p, int size = 1);

        void read(int &p);
        void write(int p);

        void read(float &p);
        void write(float p);

        std::istream &read_line_by_line(std::string &line);
        
        bool compare(const std::string &first, const std::string &other);
        bool is_line_empty(const std::string &line);

        /** @brief Splits a line into multiple strings that are between separators. 
         *  @note This algorhitm does not care about removing whitespaces from the resulting strings,
         *  if it separates using a different character other than the whitespace.
        */
        std::list<std::string> segment_line(const std::string &line, char separator = ' ');
    };
}
#endif