cmake_minimum_required(VERSION 3.0.0...3.13)

set(PROJECT_NAME UI)
set(LIBRARY_NAME UI)

set(LIBRARY_HEADERS_DIRECTORY include/${LIBRARY_NAME})
set(LIBRARY_SOURCE_DIRECTORY src)



project(${PROJECT_NAME})

add_library(${LIBRARY_NAME} STATIC
    # Headers
    ${LIBRARY_HEADERS_DIRECTORY}/Global.h

    ${LIBRARY_HEADERS_DIRECTORY}/style/Drawable.h
    ${LIBRARY_HEADERS_DIRECTORY}/style/TextureDrawable.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/Element.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/layout/Table.h
    ${LIBRARY_HEADERS_DIRECTORY}/layout/Cell.h

    ${LIBRARY_HEADERS_DIRECTORY}/Label.h
    ${LIBRARY_HEADERS_DIRECTORY}/ImageElement.h
    ${LIBRARY_HEADERS_DIRECTORY}/Button.h
    ${LIBRARY_HEADERS_DIRECTORY}/Checkbox.h
    ${LIBRARY_HEADERS_DIRECTORY}/Slider.h
    ${LIBRARY_HEADERS_DIRECTORY}/Core.h
    
    # Sources
    ${LIBRARY_SOURCE_DIRECTORY}/Global.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/Element.cpp
    
    ${LIBRARY_SOURCE_DIRECTORY}/layout/Table.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/layout/Cell.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/Label.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/ImageElement.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/Button.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/Checkbox.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/Slider.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/Core.cpp
)

target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE: include/${LIBRARY_NAME}>
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE: include>
)


target_link_libraries(${LIBRARY_NAME} PUBLIC Icosahedron)