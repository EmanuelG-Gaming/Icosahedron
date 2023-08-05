# Icosahedron
A game engine that uses SDL2 with GLEW.

# Building the code:
For some, this can be pretty straightforward, but here it is:

- On VSCode:
    - Download the C/C++ extension on [their website](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools);
    - Install MingW with CMake;
    - Clone this repository;
    - Open Visual Studio Code (VSCode);
    - (Optional) Uncomment one of the examples in `src/main.cpp`;
    - Build the code;
    - Run it.

(Assuming you've got a C++ compiler, OpenGL support on the computer, and SDL2 installed)

If you've managed to build this project successfully, you may see a `build` folder being created, with something like `engine.exe`. You can use the `.\build\engine` command on the terminal, that will start your application.

# Example usage
More examples are available inside the `src/main.cpp` file.

For instance, a very simple application that shows an empty blue window on startup can be defined as follows:

```
#include <Icosahedron/Core.h>

class Example1 : public ic::Application {
    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }
        
        bool load() override {
            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override { 
            clear_color(ic::Colors::blue);
            return true; 
        }
};

int main(int argc, char *argv[]) {
    Example1 application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
```