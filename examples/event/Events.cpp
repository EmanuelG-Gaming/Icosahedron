#include <iostream>
#include <Icosahedron/Events.h>
#include <Icosahedron/Debug.h>

template <typename T>
struct Consumer {
    void (T::*cons_member)(void);
};


class YesClass {
    public:
        void the() {
            std::cout << "Yes." << "\n";
        }
};

void event_callback() {
    std::cout << "This is a callback from an event." << "\n";
}

int main() {
    // For Windows-specific platforms, this makes a command prompt window
    ic::create_console();

    YesClass yes;


    Consumer<YesClass> c;
    c.cons_member = &YesClass::the;


    std::cout << "hello." << "\n";

    ic::Events::on("eventTest", event_callback);

    ic::Events::fire("eventTest");
    ic::Events::fire("eventTest");

    // Halt the closing of the terminal window, until text has been inputted from the keyboard, regardless of platform
    std::cin.get();

    return 0;
}