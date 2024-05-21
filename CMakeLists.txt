cmake_minimum_required(VERSION 3.0.0...3.13)

project(engine VERSION 0.4.0 LANGUAGES C CXX)
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)

# Inspired by https://github.com/Gaetz/SDL-OpenGL/blob/master/CMakeLists.txt
include(CTest)
enable_testing()

add_subdirectory("third-party/glad")
add_subdirectory("third-party/soloud")
add_subdirectory("third-party/freetype-2.13.2")

set(SDL2_DIR ${CMAKE_SOURCE_DIR}/third-party/SDL2)
set(SDL2_IMAGE_DIR ${CMAKE_SOURCE_DIR}/third-party/SDL2_image)

set(FREETYPE_DIR ${CMAKE_SOURCE_DIR}/third-party/freetype-2.13.2)


if (NOT WIN32)
    string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
    string(STRIP "${SDL2_IMAGE_LIBRARIES}" SDL2_IMAGE_LIBRARIES)
    
    string(STRIP "${FREETYPE_LIBRARIES}" FREETYPE_LIBRARIES)
endif (NOT WIN32)

set(FREETYPE_INCLUDES ${FREETYPE_DIR}/include)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(OpenGL)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

message(${FREETYPE_INCLUDES})
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${FREETYPE_INCLUDES})



# Libraries and extensions
add_subdirectory(libs/Icosahedron)
add_subdirectory(libs/Physics)
add_subdirectory(libs/UI)

##### Main program #####
add_executable(main "src/main.cpp")
target_link_libraries(main Icosahedron Physics UI ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} freetype)

##### Examples #####
add_executable(examples "examples/2D/Mesh2D.cpp")
target_link_libraries(examples Icosahedron Physics UI ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} freetype)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
