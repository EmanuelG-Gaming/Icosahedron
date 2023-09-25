#include <Examples/WindowExample.h>

int main(int argc, char *argv[]) {
    WindowExample application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}