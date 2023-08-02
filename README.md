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

# Examples
These are also available in the `src/main.cpp` file.

- 1 An application that shows an empty blue window on startup:

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

- 2 An application that presents a green rectangle that can be moved using WASD keys:

```
#include <Icosahedron/Core.h>

class Example2 : public ic::Application {
    ic::Batch2D *batch;
    ic::RectangleShape *shape;

    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }
        
        bool load() override {
            batch = new ic::Batch2D(1000, TRIANGLES);
            shape = new ic::RectangleShape();

            inputHandler.add_input(new ic::KeyboardController(), "WASD");

            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override { 
            float speed = 1.0f;
            ic::KeyboardController *controller = (ic::KeyboardController*) inputHandler.find_input("WASD");
            ic::Vector<int, 2> dir = controller->get_direction();

            shape->position[0] += dir[0] * speed * dt;
            shape->position[1] += dir[1] * speed * dt;


            clear_color(ic::Colors::blue);

            shape->draw(batch, ic::Colors::green);
            batch->render();

            return true; 
        }

        void dispose() override {
            batch->dispose();
        }
};

int main(int argc, char *argv[]) {
    Example2 application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
```