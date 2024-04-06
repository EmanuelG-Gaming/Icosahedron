#include <IcosahedronDebug/ConsoleOutput.h>

static void create_console() {
#ifdef _WIN32
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif
}

static void ic::Debug::write_file(const char *fileName, FILE *stream) {
    freopen(fileName, "w", stream);
}