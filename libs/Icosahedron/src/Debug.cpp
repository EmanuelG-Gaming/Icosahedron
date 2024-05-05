#include <Icosahedron/Debug.h>


/////////////////////
//// Debug utils ////
/////////////////////

void ic::create_console() {
#ifdef _WIN32
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif
}

void ic::write_file(const char *fileName, FILE *stream) {
    freopen(fileName, "w", stream);
}