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
| audio | Audio-type abstractions. Currently supports music that plays across the entire scene. |
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

# Extensions
Right now, Icosahedron only has the `Physics` extension.

# Building the code:
For some, this can be pretty straightforward, but here it is:

- On VSCode:
    - Download the C/C++ extension on [their website](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools), and CMake tools might come in handy;
    - Install MingW with CMake;
    - Clone this repository;
    - Open Visual Studio Code;
    - Write some code or use an example;
    - Build the project;
    - Run it.

(Assuming you've got a C++ compiler, and a computer with OpenGL >= 3.0 support)

If you've managed to build this project successfully, you may see a `build` folder being created, with something like `engine.exe` or `icosahedron-examples.exe`. You can run either `.\build\engine` or `.build\icosahedron-examples` on the terminal, and these will start your application.

# Documentation
See [https://github.com/EmanuelG-Gaming/Icosahedron/wiki](https://github.com/EmanuelG-Gaming/Icosahedron/wiki).