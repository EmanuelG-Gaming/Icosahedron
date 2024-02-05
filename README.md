![Icosahedron cover art](resources/icosahedron-cover-art.png)

# Icosahedron
A game engine that uses OpenGL for graphics. It is supposed to be for both 2D and 3D applications.

# Links:
Discord server: [https://discord.gg/jCCEGpds6V](https://discord.gg/jCCEGpds6V)

# Extensions
Right now, Icosahedron has the `Physics` and `UI` extensions.

# Building the code:
For some, this can be pretty straightforward, but here it is:

## On Windows, via Visual studio code (VScode):
- Download the C/C++ extension on [their website](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools), and CMake  tools might come in handy;
- Install MingW with CMake;
- Clone this repository;
- Open Visual Studio Code;
- Write some code or use an example;
- Build the project;
- Run it.

## On Linux:
To get started, run the following commands to install the dependencies:

For Arch-based (including Arch itself) distros:
```
sudo pacman -S cmake
sudo pacman -S sdl2 sdl2_image
```

Then, create a folder named "build" in the root directory of the project:
```
mkdir build
```

To get the configuration files, go to the build folder and execute:
```
cd build
cmake ..
```

To compile the executable files, do:
```
cmake --build .
```

There are two executables being created.
 


(Assuming you've got a C++ compiler, and a computer with OpenGL >= 3.0 support)

If you've managed to build this project successfully, you may see a `build` folder with executables named `examples` and `main`.
On Windows, you can run `./build/main.exe` or `./build/examples.exe` to start your application.
On Linux, you can do `./build/main` or `./build/examples` on the root directory to start your application.

# Documentation
See [https://github.com/EmanuelG-Gaming/Icosahedron/wiki](https://github.com/EmanuelG-Gaming/Icosahedron/wiki).
