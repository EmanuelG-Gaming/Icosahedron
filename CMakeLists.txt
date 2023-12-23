cmake_minimum_required(VERSION 3.0.0)

project(engine VERSION 0.4.0 LANGUAGES C CXX)


add_subdirectory("third-party/glad")
add_subdirectory("third-party/soloud")

if (WIN32)
    set(SDL2_DIR ${CMAKE_SOURCE_DIR}/third-party/SDL2)
    set(SDL2_IMAGE_DIR ${CMAKE_SOURCE_DIR}/third-party/SDL2_image)
    
    set(FREETYPE2_DIR ${CMAKE_SOURCE_DIR}/third-party/freetype2)
endif (WIN32)

include(${SDL2_DIR}/sdl2-config.cmake)
include(${SDL2_IMAGE_DIR}/sdl2-image-config.cmake)
include(${FREETYPE2_DIR}/freetype2-config.cmake)


if (NOT WIN32)
    string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
    string(STRIP ${SDL2_IMAGE_LIBRARIES} SDL2_IMAGE_LIBRARIES)
    
    string(STRIP ${FREETYPE2_LIBRARIES} FREETYPE2_LIBRARIES)
endif (NOT WIN32)


set(SDL2_LIBRARIES ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
set(FREETYPE2_LIBRARIES ${FREETYPE2_LIBRARIES})

set(SDL2_INCLUDES ${SDL2_DIR}/include ${SDL2_IMAGE_DIR}/include)
set(FREETYPE2_INCLUDES ${FREETYPE2_DIR}/include)



# Libraries and extensions
add_subdirectory(libs/Icosahedron)
add_subdirectory(libs/Physics)
add_subdirectory(libs/UI)




# Build two programs having the same libraries

##### Main part #####
add_executable(icosahedron-main "src/main.cpp")
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(OpenGL)

link_directories(${SDL2_LIBRARIES}
    ${FREETYPE2_LIBRARIES}
)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${SDL2_INCLUDES}
    ${FREETYPE2_INCLUDES}
)

target_link_libraries(icosahedron-main PUBLIC freetype)
target_include_directories(icosahedron-main PRIVATE ${FREETYPE2_INCLUDES})

target_link_libraries(icosahedron-main PUBLIC Icosahedron Physics UI)


##### Examples #####
add_executable(icosahedron-examples "examples/demos/SokobanDemo.cpp")

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(OpenGL)

link_directories(${SDL2_LIBRARIES} ${FREETYPE2_LIBRARIES})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${SDL2_INCLUDES} ${FREETYPE2_INCLUDES})

target_link_libraries(icosahedron-examples PUBLIC freetype)
target_include_directories(icosahedron-examples PRIVATE ${FREETYPE2_INCLUDES})

target_link_libraries(icosahedron-examples PUBLIC Icosahedron Physics UI)

set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)