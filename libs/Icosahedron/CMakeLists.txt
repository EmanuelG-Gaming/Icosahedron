cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME IcosahedronLibrary)
set(LIBRARY_NAME Icosahedron)

set(LIBRARY_HEADERS_DIRECTORY include/${LIBRARY_NAME})
set(LIBRARY_HEADERS 
    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Vector.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/Color.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/Colors.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Shader.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Batch.h

    ${LIBRARY_HEADERS_DIRECTORY}/input/Input.h
    ${LIBRARY_HEADERS_DIRECTORY}/input/InputHandler.h
    ${LIBRARY_HEADERS_DIRECTORY}/input/KeyboardController.h

    ${LIBRARY_HEADERS_DIRECTORY}/scene/2d/Shape2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/scene/2d/RectangleShape.h

    ${LIBRARY_HEADERS_DIRECTORY}/Application.h
    ${LIBRARY_HEADERS_DIRECTORY}/Core.h
)

set(LIBRARY_SOURCE_DIRECTORY src)
set(LIBRARY_SOURCE
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/Shader.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/Batch.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/scene/2d/RectangleShape.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/input/KeyboardController.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/input/InputHandler.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/Application.cpp
)


project(${PROJECT_NAME})

add_library(${LIBRARY_NAME} STATIC
    ${LIBRARY_HEADERS}
    ${LIBRARY_SOURCE}
)

target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE: include/${LIBRARY_NAME}>
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE: include>
)