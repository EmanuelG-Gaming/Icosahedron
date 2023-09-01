![Icosahedron cover art](resources/icosahedron-cover-art.png)

# Icosahedron
A game engine that uses OpenGL. It is supposed to be for both 2D and 3D applications.

# Library structure
The following table's contents may be prone to change:

| Directory name | Description |
| --- | --- |
| graphics | For things graphics-related. Also contains a FreeType 2 wrapper that is made to render text to a `TextAtlas`. |
| graphics/gl | OpenGL wrappers. |
| files | Filesystem. |
| input | Contains controllers for input devices such as your keyboard or your mouse. These are added and handled via `InputHandler`. |
| math | Header-only definitions to mathematical objects and functions. |
| math/geom | Same thing, but for geometrical concepts such as the vector, or the axis-aligned rectangle. |
| scene | Contains both 2D and 3D elements of an usual scene. |
| scene/controllers | Preset controllers for scene objects, such as the `Camera3D`. |
| util | Utilities, like the `GeometryGenerator`. |
| assets | Asset loaders. |

The rest of the files that may not belong to the folders mentioned above are `Application`,  `Core`, and `Renderer`.

- `Application` is the starting point for an usual application made with Icosahedron;
- `Core` contains include paths to almost all the headers of this library;
- `Renderer` has functions for rendering geometry;
- `Global` provides entry for global-scope variables, like the window size via `IC_WINDOW_WIDTH` and `IC_WINDOW_HEIGHT`.

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

(Assuming you've got a C++ compiler, OpenGL >= 3.0 support on the computer, and SDL2 installed)

If you've managed to build this project successfully, you may see a `build` folder being created, with something like `engine.exe`. You can use the `.\build\engine` command on the terminal, and this will start your application.

# Example usage
More examples are available inside the `src/main.cpp` file.

For instance, a very simple application that shows an empty blue window on startup can be defined as follows:

```
#include <Icosahedron/Core.h>

class Example : public ic::Application {
    public:
        bool init() override {
            // Use this to set up window settings, although this can also be done in the constructor
            displayName = "Example window";
            
            return true;
        }
        
        bool load() override {
            // This function is called after init() and after setting up the window

            return true;
        }

        void window_size_changed(int w, int h) override {
            // Called when the window is resized, and also if it is changed to fullscreen mode
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            // Called when events are retrieved and polled 

            return true;
        }

        bool update(float dt) override {
            // This is called once every frame
        
            clear_color(ic::Colors::blue);
            return true; 
        }

        void dispose() override {
            // Occurs when the application has to close

        }
};

int main(int argc, char *argv[]) {
    Example application;

    // Constructs a window that is 640 pixels wide and 480 pixels tall
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
```