# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\msys2\mingw64\bin\cmake.exe

# The command to remove a file.
RM = C:\msys2\mingw64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Icosahedron

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Icosahedron\build

# Include any dependencies generated for this target.
include libs/Physics/CMakeFiles/Physics.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include libs/Physics/CMakeFiles/Physics.dir/compiler_depend.make

# Include the progress variables for this target.
include libs/Physics/CMakeFiles/Physics.dir/progress.make

# Include the compile flags for this target's objects.
include libs/Physics/CMakeFiles/Physics.dir/flags.make

libs/Physics/CMakeFiles/Physics.dir/src/2d/Transform2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/flags.make
libs/Physics/CMakeFiles/Physics.dir/src/2d/Transform2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/includes_CXX.rsp
libs/Physics/CMakeFiles/Physics.dir/src/2d/Transform2D.cpp.obj: C:/Icosahedron/libs/Physics/src/2d/Transform2D.cpp
libs/Physics/CMakeFiles/Physics.dir/src/2d/Transform2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Icosahedron\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libs/Physics/CMakeFiles/Physics.dir/src/2d/Transform2D.cpp.obj"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/Physics/CMakeFiles/Physics.dir/src/2d/Transform2D.cpp.obj -MF CMakeFiles\Physics.dir\src\2d\Transform2D.cpp.obj.d -o CMakeFiles\Physics.dir\src\2d\Transform2D.cpp.obj -c C:\Icosahedron\libs\Physics\src\2d\Transform2D.cpp

libs/Physics/CMakeFiles/Physics.dir/src/2d/Transform2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Physics.dir/src/2d/Transform2D.cpp.i"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Icosahedron\libs\Physics\src\2d\Transform2D.cpp > CMakeFiles\Physics.dir\src\2d\Transform2D.cpp.i

libs/Physics/CMakeFiles/Physics.dir/src/2d/Transform2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Physics.dir/src/2d/Transform2D.cpp.s"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Icosahedron\libs\Physics\src\2d\Transform2D.cpp -o CMakeFiles\Physics.dir\src\2d\Transform2D.cpp.s

libs/Physics/CMakeFiles/Physics.dir/src/2d/Collider2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/flags.make
libs/Physics/CMakeFiles/Physics.dir/src/2d/Collider2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/includes_CXX.rsp
libs/Physics/CMakeFiles/Physics.dir/src/2d/Collider2D.cpp.obj: C:/Icosahedron/libs/Physics/src/2d/Collider2D.cpp
libs/Physics/CMakeFiles/Physics.dir/src/2d/Collider2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Icosahedron\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object libs/Physics/CMakeFiles/Physics.dir/src/2d/Collider2D.cpp.obj"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/Physics/CMakeFiles/Physics.dir/src/2d/Collider2D.cpp.obj -MF CMakeFiles\Physics.dir\src\2d\Collider2D.cpp.obj.d -o CMakeFiles\Physics.dir\src\2d\Collider2D.cpp.obj -c C:\Icosahedron\libs\Physics\src\2d\Collider2D.cpp

libs/Physics/CMakeFiles/Physics.dir/src/2d/Collider2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Physics.dir/src/2d/Collider2D.cpp.i"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Icosahedron\libs\Physics\src\2d\Collider2D.cpp > CMakeFiles\Physics.dir\src\2d\Collider2D.cpp.i

libs/Physics/CMakeFiles/Physics.dir/src/2d/Collider2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Physics.dir/src/2d/Collider2D.cpp.s"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Icosahedron\libs\Physics\src\2d\Collider2D.cpp -o CMakeFiles\Physics.dir\src\2d\Collider2D.cpp.s

libs/Physics/CMakeFiles/Physics.dir/src/2d/CollisionUtils2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/flags.make
libs/Physics/CMakeFiles/Physics.dir/src/2d/CollisionUtils2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/includes_CXX.rsp
libs/Physics/CMakeFiles/Physics.dir/src/2d/CollisionUtils2D.cpp.obj: C:/Icosahedron/libs/Physics/src/2d/CollisionUtils2D.cpp
libs/Physics/CMakeFiles/Physics.dir/src/2d/CollisionUtils2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Icosahedron\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object libs/Physics/CMakeFiles/Physics.dir/src/2d/CollisionUtils2D.cpp.obj"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/Physics/CMakeFiles/Physics.dir/src/2d/CollisionUtils2D.cpp.obj -MF CMakeFiles\Physics.dir\src\2d\CollisionUtils2D.cpp.obj.d -o CMakeFiles\Physics.dir\src\2d\CollisionUtils2D.cpp.obj -c C:\Icosahedron\libs\Physics\src\2d\CollisionUtils2D.cpp

libs/Physics/CMakeFiles/Physics.dir/src/2d/CollisionUtils2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Physics.dir/src/2d/CollisionUtils2D.cpp.i"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Icosahedron\libs\Physics\src\2d\CollisionUtils2D.cpp > CMakeFiles\Physics.dir\src\2d\CollisionUtils2D.cpp.i

libs/Physics/CMakeFiles/Physics.dir/src/2d/CollisionUtils2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Physics.dir/src/2d/CollisionUtils2D.cpp.s"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Icosahedron\libs\Physics\src\2d\CollisionUtils2D.cpp -o CMakeFiles\Physics.dir\src\2d\CollisionUtils2D.cpp.s

libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/Object2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/flags.make
libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/Object2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/includes_CXX.rsp
libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/Object2D.cpp.obj: C:/Icosahedron/libs/Physics/src/2d/objects/Object2D.cpp
libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/Object2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Icosahedron\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/Object2D.cpp.obj"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/Object2D.cpp.obj -MF CMakeFiles\Physics.dir\src\2d\objects\Object2D.cpp.obj.d -o CMakeFiles\Physics.dir\src\2d\objects\Object2D.cpp.obj -c C:\Icosahedron\libs\Physics\src\2d\objects\Object2D.cpp

libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/Object2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Physics.dir/src/2d/objects/Object2D.cpp.i"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Icosahedron\libs\Physics\src\2d\objects\Object2D.cpp > CMakeFiles\Physics.dir\src\2d\objects\Object2D.cpp.i

libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/Object2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Physics.dir/src/2d/objects/Object2D.cpp.s"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Icosahedron\libs\Physics\src\2d\objects\Object2D.cpp -o CMakeFiles\Physics.dir\src\2d\objects\Object2D.cpp.s

libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/RigidObject2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/flags.make
libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/RigidObject2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/includes_CXX.rsp
libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/RigidObject2D.cpp.obj: C:/Icosahedron/libs/Physics/src/2d/objects/RigidObject2D.cpp
libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/RigidObject2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Icosahedron\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/RigidObject2D.cpp.obj"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/RigidObject2D.cpp.obj -MF CMakeFiles\Physics.dir\src\2d\objects\RigidObject2D.cpp.obj.d -o CMakeFiles\Physics.dir\src\2d\objects\RigidObject2D.cpp.obj -c C:\Icosahedron\libs\Physics\src\2d\objects\RigidObject2D.cpp

libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/RigidObject2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Physics.dir/src/2d/objects/RigidObject2D.cpp.i"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Icosahedron\libs\Physics\src\2d\objects\RigidObject2D.cpp > CMakeFiles\Physics.dir\src\2d\objects\RigidObject2D.cpp.i

libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/RigidObject2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Physics.dir/src/2d/objects/RigidObject2D.cpp.s"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Icosahedron\libs\Physics\src\2d\objects\RigidObject2D.cpp -o CMakeFiles\Physics.dir\src\2d\objects\RigidObject2D.cpp.s

libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/SpringMassSystem2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/flags.make
libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/SpringMassSystem2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/includes_CXX.rsp
libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/SpringMassSystem2D.cpp.obj: C:/Icosahedron/libs/Physics/src/2d/objects/SpringMassSystem2D.cpp
libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/SpringMassSystem2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Icosahedron\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/SpringMassSystem2D.cpp.obj"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/SpringMassSystem2D.cpp.obj -MF CMakeFiles\Physics.dir\src\2d\objects\SpringMassSystem2D.cpp.obj.d -o CMakeFiles\Physics.dir\src\2d\objects\SpringMassSystem2D.cpp.obj -c C:\Icosahedron\libs\Physics\src\2d\objects\SpringMassSystem2D.cpp

libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/SpringMassSystem2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Physics.dir/src/2d/objects/SpringMassSystem2D.cpp.i"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Icosahedron\libs\Physics\src\2d\objects\SpringMassSystem2D.cpp > CMakeFiles\Physics.dir\src\2d\objects\SpringMassSystem2D.cpp.i

libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/SpringMassSystem2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Physics.dir/src/2d/objects/SpringMassSystem2D.cpp.s"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Icosahedron\libs\Physics\src\2d\objects\SpringMassSystem2D.cpp -o CMakeFiles\Physics.dir\src\2d\objects\SpringMassSystem2D.cpp.s

libs/Physics/CMakeFiles/Physics.dir/src/2d/Solver2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/flags.make
libs/Physics/CMakeFiles/Physics.dir/src/2d/Solver2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/includes_CXX.rsp
libs/Physics/CMakeFiles/Physics.dir/src/2d/Solver2D.cpp.obj: C:/Icosahedron/libs/Physics/src/2d/Solver2D.cpp
libs/Physics/CMakeFiles/Physics.dir/src/2d/Solver2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Icosahedron\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object libs/Physics/CMakeFiles/Physics.dir/src/2d/Solver2D.cpp.obj"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/Physics/CMakeFiles/Physics.dir/src/2d/Solver2D.cpp.obj -MF CMakeFiles\Physics.dir\src\2d\Solver2D.cpp.obj.d -o CMakeFiles\Physics.dir\src\2d\Solver2D.cpp.obj -c C:\Icosahedron\libs\Physics\src\2d\Solver2D.cpp

libs/Physics/CMakeFiles/Physics.dir/src/2d/Solver2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Physics.dir/src/2d/Solver2D.cpp.i"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Icosahedron\libs\Physics\src\2d\Solver2D.cpp > CMakeFiles\Physics.dir\src\2d\Solver2D.cpp.i

libs/Physics/CMakeFiles/Physics.dir/src/2d/Solver2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Physics.dir/src/2d/Solver2D.cpp.s"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Icosahedron\libs\Physics\src\2d\Solver2D.cpp -o CMakeFiles\Physics.dir\src\2d\Solver2D.cpp.s

libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/Gravity2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/flags.make
libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/Gravity2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/includes_CXX.rsp
libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/Gravity2D.cpp.obj: C:/Icosahedron/libs/Physics/src/2d/forces/Gravity2D.cpp
libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/Gravity2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Icosahedron\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/Gravity2D.cpp.obj"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/Gravity2D.cpp.obj -MF CMakeFiles\Physics.dir\src\2d\forces\Gravity2D.cpp.obj.d -o CMakeFiles\Physics.dir\src\2d\forces\Gravity2D.cpp.obj -c C:\Icosahedron\libs\Physics\src\2d\forces\Gravity2D.cpp

libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/Gravity2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Physics.dir/src/2d/forces/Gravity2D.cpp.i"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Icosahedron\libs\Physics\src\2d\forces\Gravity2D.cpp > CMakeFiles\Physics.dir\src\2d\forces\Gravity2D.cpp.i

libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/Gravity2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Physics.dir/src/2d/forces/Gravity2D.cpp.s"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Icosahedron\libs\Physics\src\2d\forces\Gravity2D.cpp -o CMakeFiles\Physics.dir\src\2d\forces\Gravity2D.cpp.s

libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/WindForce2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/flags.make
libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/WindForce2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/includes_CXX.rsp
libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/WindForce2D.cpp.obj: C:/Icosahedron/libs/Physics/src/2d/forces/WindForce2D.cpp
libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/WindForce2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Icosahedron\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/WindForce2D.cpp.obj"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/WindForce2D.cpp.obj -MF CMakeFiles\Physics.dir\src\2d\forces\WindForce2D.cpp.obj.d -o CMakeFiles\Physics.dir\src\2d\forces\WindForce2D.cpp.obj -c C:\Icosahedron\libs\Physics\src\2d\forces\WindForce2D.cpp

libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/WindForce2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Physics.dir/src/2d/forces/WindForce2D.cpp.i"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Icosahedron\libs\Physics\src\2d\forces\WindForce2D.cpp > CMakeFiles\Physics.dir\src\2d\forces\WindForce2D.cpp.i

libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/WindForce2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Physics.dir/src/2d/forces/WindForce2D.cpp.s"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Icosahedron\libs\Physics\src\2d\forces\WindForce2D.cpp -o CMakeFiles\Physics.dir\src\2d\forces\WindForce2D.cpp.s

libs/Physics/CMakeFiles/Physics.dir/src/2d/levels/PhysicsLevel2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/flags.make
libs/Physics/CMakeFiles/Physics.dir/src/2d/levels/PhysicsLevel2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/includes_CXX.rsp
libs/Physics/CMakeFiles/Physics.dir/src/2d/levels/PhysicsLevel2D.cpp.obj: C:/Icosahedron/libs/Physics/src/2d/levels/PhysicsLevel2D.cpp
libs/Physics/CMakeFiles/Physics.dir/src/2d/levels/PhysicsLevel2D.cpp.obj: libs/Physics/CMakeFiles/Physics.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Icosahedron\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object libs/Physics/CMakeFiles/Physics.dir/src/2d/levels/PhysicsLevel2D.cpp.obj"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/Physics/CMakeFiles/Physics.dir/src/2d/levels/PhysicsLevel2D.cpp.obj -MF CMakeFiles\Physics.dir\src\2d\levels\PhysicsLevel2D.cpp.obj.d -o CMakeFiles\Physics.dir\src\2d\levels\PhysicsLevel2D.cpp.obj -c C:\Icosahedron\libs\Physics\src\2d\levels\PhysicsLevel2D.cpp

libs/Physics/CMakeFiles/Physics.dir/src/2d/levels/PhysicsLevel2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Physics.dir/src/2d/levels/PhysicsLevel2D.cpp.i"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Icosahedron\libs\Physics\src\2d\levels\PhysicsLevel2D.cpp > CMakeFiles\Physics.dir\src\2d\levels\PhysicsLevel2D.cpp.i

libs/Physics/CMakeFiles/Physics.dir/src/2d/levels/PhysicsLevel2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Physics.dir/src/2d/levels/PhysicsLevel2D.cpp.s"
	cd /d C:\Icosahedron\build\libs\Physics && C:\msys2\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Icosahedron\libs\Physics\src\2d\levels\PhysicsLevel2D.cpp -o CMakeFiles\Physics.dir\src\2d\levels\PhysicsLevel2D.cpp.s

# Object files for target Physics
Physics_OBJECTS = \
"CMakeFiles/Physics.dir/src/2d/Transform2D.cpp.obj" \
"CMakeFiles/Physics.dir/src/2d/Collider2D.cpp.obj" \
"CMakeFiles/Physics.dir/src/2d/CollisionUtils2D.cpp.obj" \
"CMakeFiles/Physics.dir/src/2d/objects/Object2D.cpp.obj" \
"CMakeFiles/Physics.dir/src/2d/objects/RigidObject2D.cpp.obj" \
"CMakeFiles/Physics.dir/src/2d/objects/SpringMassSystem2D.cpp.obj" \
"CMakeFiles/Physics.dir/src/2d/Solver2D.cpp.obj" \
"CMakeFiles/Physics.dir/src/2d/forces/Gravity2D.cpp.obj" \
"CMakeFiles/Physics.dir/src/2d/forces/WindForce2D.cpp.obj" \
"CMakeFiles/Physics.dir/src/2d/levels/PhysicsLevel2D.cpp.obj"

# External object files for target Physics
Physics_EXTERNAL_OBJECTS =

libs/Physics/libPhysics.a: libs/Physics/CMakeFiles/Physics.dir/src/2d/Transform2D.cpp.obj
libs/Physics/libPhysics.a: libs/Physics/CMakeFiles/Physics.dir/src/2d/Collider2D.cpp.obj
libs/Physics/libPhysics.a: libs/Physics/CMakeFiles/Physics.dir/src/2d/CollisionUtils2D.cpp.obj
libs/Physics/libPhysics.a: libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/Object2D.cpp.obj
libs/Physics/libPhysics.a: libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/RigidObject2D.cpp.obj
libs/Physics/libPhysics.a: libs/Physics/CMakeFiles/Physics.dir/src/2d/objects/SpringMassSystem2D.cpp.obj
libs/Physics/libPhysics.a: libs/Physics/CMakeFiles/Physics.dir/src/2d/Solver2D.cpp.obj
libs/Physics/libPhysics.a: libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/Gravity2D.cpp.obj
libs/Physics/libPhysics.a: libs/Physics/CMakeFiles/Physics.dir/src/2d/forces/WindForce2D.cpp.obj
libs/Physics/libPhysics.a: libs/Physics/CMakeFiles/Physics.dir/src/2d/levels/PhysicsLevel2D.cpp.obj
libs/Physics/libPhysics.a: libs/Physics/CMakeFiles/Physics.dir/build.make
libs/Physics/libPhysics.a: libs/Physics/CMakeFiles/Physics.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Icosahedron\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX static library libPhysics.a"
	cd /d C:\Icosahedron\build\libs\Physics && $(CMAKE_COMMAND) -P CMakeFiles\Physics.dir\cmake_clean_target.cmake
	cd /d C:\Icosahedron\build\libs\Physics && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Physics.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/Physics/CMakeFiles/Physics.dir/build: libs/Physics/libPhysics.a
.PHONY : libs/Physics/CMakeFiles/Physics.dir/build

libs/Physics/CMakeFiles/Physics.dir/clean:
	cd /d C:\Icosahedron\build\libs\Physics && $(CMAKE_COMMAND) -P CMakeFiles\Physics.dir\cmake_clean.cmake
.PHONY : libs/Physics/CMakeFiles/Physics.dir/clean

libs/Physics/CMakeFiles/Physics.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Icosahedron C:\Icosahedron\libs\Physics C:\Icosahedron\build C:\Icosahedron\build\libs\Physics C:\Icosahedron\build\libs\Physics\CMakeFiles\Physics.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : libs/Physics/CMakeFiles/Physics.dir/depend

