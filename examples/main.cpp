#include <Examples/MultipleViewports.h>

int main(int argc, char *argv[]) {
    MultipleViewports application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}