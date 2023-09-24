#include <Examples/TiledDemo.h>

int main(int argc, char *argv[]) {
    TiledDemo application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}