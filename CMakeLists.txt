cmake_minimum_required(VERSION 3.0.0)

# Setting up constants #
set(ENGINE_PROJECT_NAME engine)
set(ENGINE_FULL_PROJECT_NAME ${ENGINE_PROJECT_NAME} VERSION 0.1.0 LANGUAGES C CXX)

set(ENGINE_SOURCE_DIRECTORY src)
set(ENGINE_SOURCE ${ENGINE_SOURCE_DIRECTORY}/main.cpp)

set(ENGINE_LIBRARIES_DIRECTORY libs)
set(ENGINE_LIBRARIES Icosahedron)

project(${ENGINE_FULL_PROJECT_NAME})

# Testing #
include(CTest)
enable_testing()

# Project #
add_executable(${ENGINE_PROJECT_NAME} ${ENGINE_SOURCE})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

foreach(LIBRARY ${ENGINE_LIBRARIES})
    add_subdirectory("${ENGINE_LIBRARIES_DIRECTORY}/${LIBRARY}")
endforeach(LIBRARY)
target_link_libraries(${ENGINE_PROJECT_NAME} ${ENGINE_LIBRARIES})


### Might have included these from the third-party vendors folder ###
# GLEW #
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(${ENGINE_PROJECT_NAME} GLEW::glew)

# SDL2 main #
find_package(SDL2 REQUIRED SDL2main)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${ENGINE_PROJECT_NAME} SDL2::SDL2main)

# SDL2 #
find_package(SDL2 REQUIRED SDL2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${ENGINE_PROJECT_NAME} SDL2::SDL2)


