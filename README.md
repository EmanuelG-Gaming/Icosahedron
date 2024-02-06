![Icosahedron cover art](resources/icosahedron-cover-art.png)

# Icosahedron
A game engine that uses OpenGL for graphics. It is supposed to be for both 2D and 3D applications.

# Links:
Discord server: [https://discord.gg/jCCEGpds6V](https://discord.gg/jCCEGpds6V)

# Extensions
Right now, Icosahedron has the `Physics` and `UI` extensions.

# Building the code:
For some, this can be pretty straightforward, but here it is:

(Assuming you've got OpenGL >= 3.0 support on your computer)

## On Windows
### Visual studio code (VScode):

- Download the C/C++ extension on [their website](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools), and CMake  tools might come in handy;
- Install MingW with CMake;
- Clone this repository;
- Open Visual Studio Code;
- Write some code or use an example;
- Build the project;
- Run it.

## On Linux:

To get started, run the following commands to install the dependencies:

### For Arch-distributions:
```
sudo pacman -S cmake g++
sudo pacman -S sdl2 sdl2_image
```

### For Debian-distributions:
```
sudo apt install cmake g++
sudo apt install libsdl2-dev libsdl2-2.0-0
sudo apt install libsdl2-image-dev libsdl2-image-2.0-0
```

If you haven't got OpenGL installed, especially on ( virtual machine? ) Debian, do so by typing this command to install the MESA OGL driver:

```
sudo apt install libgl1-mesa-dev
```



### The actual building:
Clone this repository by clicking the green button above the engine's file system or by typing the following command:
```
git clone https://github.com/EmanuelG-Gaming/Icoshedron
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




### Note:

If you've managed to build this project successfully, you may see a `build` folder with executables named `examples` and `main`.
On Windows, you can run `./build/main.exe` or `./build/examples.exe` to start your application.
On Linux, you can do `./build/main` or `./build/examples` on the project's directory to start your application.

# Documentation
See [https://github.com/EmanuelG-Gaming/Icosahedron/wiki](https://github.com/EmanuelG-Gaming/Icosahedron/wiki).
