#include <Icosahedron/util/IO.h>
#include <Icosahedron/util/Time.h>


void ic::IO::open(const char *fileName, std::ios::openmode writeMode) {
    ic::IO::readHandler.open(fileName, std::ios::binary);
    if (!ic::IO::readHandler.is_open() || ic::IO::readHandler.fail()) {
        printf("Couldn't read from the file %s. Maybe try to correctly type its filename?\n", fileName);
    }

    ic::IO::writeHandler.open(fileName, writeMode);
    if (!ic::IO::writeHandler.is_open() || ic::IO::writeHandler.fail()) {
        printf("Couldn't write to the file %s. Maybe try to correctly type its filename?\n", fileName);
    }
}


void ic::IO::close() {
    ic::IO::readHandler.close();
    ic::IO::writeHandler.close();
}


void ic::IO::read(const void *p, int size) {
    ic::IO::readHandler.read(reinterpret_cast<char*>(&p), size);
}

void ic::IO::write(const void *p, int size) {
    ic::IO::writeHandler.write(reinterpret_cast<const char*>(p), size);
}


void ic::IO::read(int &p) {
    ic::IO::readHandler >> p;
}

void ic::IO::write(int p) {
    ic::IO::writeHandler << p;
}

void ic::IO::read(float &p) {
    ic::IO::readHandler >> p;
}

void ic::IO::write(float p) {
    ic::IO::writeHandler << p;
}


std::istream &ic::IO::read_line_by_line(std::string &line) {
    return std::getline(ic::IO::readHandler, line);
}


bool ic::IO::compare(const std::string &first, const std::string &other) {
    return !first.compare(other);
}

bool ic::IO::is_line_empty(const std::string &line) {
    bool empty = compare(line, "") ||
                 compare(line, " ");
                  
    return empty;
}

std::list<std::string> ic::IO::segment_line(const std::string &line, char separator) {
    std::list<std::string> result;

    std::istringstream stream(line);
    std::string token;

    while (std::getline(stream, token, separator)) {
        result.push_back(token);
    }

    return result;
}