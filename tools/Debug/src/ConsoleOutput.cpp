#include <IcosahedronDebug/ConsoleOutput.h>

void ic::Debug::ConsoleOutput::write_file(const std::string &fileName, FILE *stream) {
    freopen(fileName.c_str(), "w", stream);
}