cmake_minimum_required(VERSION 3.0.0)

# Setting up constants #
set(ENGINE_PROJECT_NAME engine)
set(ENGINE_FULL_PROJECT_NAME ${ENGINE_PROJECT_NAME} VERSION 0.3.5 LANGUAGES C CXX)

set(ENGINE_SOURCE_DIRECTORY src)
set(ENGINE_SOURCE ${ENGINE_SOURCE_DIRECTORY}/main.cpp)

set(ENGINE_LIBRARIES_DIRECTORY libs)
set(ENGINE_LIBRARIES 
    Icosahedron
)

set(ENGINE_THIRD_PARTY_DIRECTORY third-party)

set(ENGINE_EXAMPLES_DIRECTORY examples)

# Project
project(${ENGINE_FULL_PROJECT_NAME})
project(icosahedron)

set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1) # Prevent the addition of multiple tests
include(CTest)
enable_testing()



# Subdirectories
add_subdirectory("${ENGINE_THIRD_PARTY_DIRECTORY}/glad")
add_subdirectory("${ENGINE_THIRD_PARTY_DIRECTORY}/soloud")
add_subdirectory("${ENGINE_THIRD_PARTY_DIRECTORY}/testing")

if (WIN32)
    set(SDL2_DIR ${CMAKE_SOURCE_DIR}/third-party/SDL2)
    set(SDL2_IMAGE_DIR ${CMAKE_SOURCE_DIR}/third-party/SDL2_image)
    
    set(FREETYPE2_DIR ${CMAKE_SOURCE_DIR}/third-party/freetype2)
endif (WIN32)

include(${SDL2_DIR}/sdl2-config.cmake)
include(${SDL2_IMAGE_DIR}/sdl2-image-config.cmake)
include(${FREETYPE2_DIR}/freetype2-config.cmake)



##### Main file #####
add_executable(${ENGINE_PROJECT_NAME} ${ENGINE_SOURCE})

# Third-party libraries #
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

# Test #
target_link_libraries(${ENGINE_PROJECT_NAME} testing)

find_package(OpenGL)


if (NOT WIN32)
    string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
    string(STRIP ${SDL2_IMAGE_LIBRARIES} SDL2_IMAGE_LIBRARIES)
    
    string(STRIP ${FREETYPE2_LIBRARIES} FREETYPE2_LIBRARIES)
endif (NOT WIN32)


set(SDL2_LIBRARIES ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
set(FREETYPE2_LIBRARIES ${FREETYPE2_LIBRARIES})

set(SDL2_INCLUDES ${SDL2_DIR}/include ${SDL2_IMAGE_DIR}/include)
set(FREETYPE2_INCLUDES ${FREETYPE2_DIR}/include)


link_directories(${SDL2_LIBRARIES}
    ${FREETYPE2_LIBRARIES}
)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${SDL2_INCLUDES}
    ${FREETYPE2_INCLUDES}
)

target_link_libraries(${ENGINE_PROJECT_NAME} freetype)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE ${FREETYPE2_INCLUDES})

# Icosahedron library
foreach(LIBRARY ${ENGINE_LIBRARIES})
    add_subdirectory("${ENGINE_LIBRARIES_DIRECTORY}/${LIBRARY}")
endforeach(LIBRARY)


target_link_libraries(${ENGINE_PROJECT_NAME} ${ENGINE_LIBRARIES})


##### Examples #####
add_executable(icosahedron-examples examples/main.cpp)


# Third-party libraries #
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

# Test #
target_link_libraries(icosahedron-examples testing)

find_package(OpenGL)


if (NOT WIN32)
    string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
    string(STRIP ${SDL2_IMAGE_LIBRARIES} SDL2_IMAGE_LIBRARIES)
    
    string(STRIP ${FREETYPE2_LIBRARIES} FREETYPE2_LIBRARIES)
endif (NOT WIN32)


set(SDL2_LIBRARIES ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
set(FREETYPE2_LIBRARIES ${FREETYPE2_LIBRARIES})

set(SDL2_INCLUDES ${SDL2_DIR}/include ${SDL2_IMAGE_DIR}/include)
set(FREETYPE2_INCLUDES ${FREETYPE2_DIR}/include)


link_directories(${SDL2_LIBRARIES}
    ${FREETYPE2_LIBRARIES}
)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${SDL2_INCLUDES}
    ${FREETYPE2_INCLUDES}
)

target_link_libraries(icosahedron-examples freetype)
target_include_directories(icosahedron-examples PRIVATE ${FREETYPE2_INCLUDES})

# Icosahedron library
foreach(LIBRARY ${ENGINE_LIBRARIES})
    #add_subdirectory("${ENGINE_LIBRARIES_DIRECTORY}/${LIBRARY}")
endforeach(LIBRARY)



target_include_directories(icosahedron-examples PRIVATE examples/include)
target_link_libraries(icosahedron-examples ${ENGINE_LIBRARIES})



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)