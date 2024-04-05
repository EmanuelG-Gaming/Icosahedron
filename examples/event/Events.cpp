#include <iostream>
#include <Icosahedron/Events.h>
#include <IcosahedronDebug/ConsoleOutput.h>


void event_callback() {
    std::cout << "This is a callback from an event.\n";
}

int main() {
    // For Windows-specific platforms, this makes a command prompt window
    ic::Debug::create_console();

    std::cout << "hello." << "\n";

    ic::Events::on("eventTest", event_callback);
    ic::Events::fire("eventTest");
    ic::Events::fire("eventTest");

    // Halt the closing of the terminal window, until text has been inputted from the keyboard, regardless of platform
    std::cin.get();

    return 0;
}