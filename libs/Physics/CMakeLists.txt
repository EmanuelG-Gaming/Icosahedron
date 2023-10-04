cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME Physics)
set(LIBRARY_NAME Physics)

set(LIBRARY_HEADERS_DIRECTORY include/${LIBRARY_NAME})
set(LIBRARY_SOURCE_DIRECTORY src)



project(${PROJECT_NAME})

add_library(${LIBRARY_NAME} STATIC
    # Headers
    ${LIBRARY_HEADERS_DIRECTORY}/2d/Transform2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/2d/ManifoldPoints2D.h

    ${LIBRARY_HEADERS_DIRECTORY}/2d/Collider2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/2d/CollisionUtils2D.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/2d/objects/Object2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/2d/objects/RigidObject2D.h


    ${LIBRARY_HEADERS_DIRECTORY}/2d/Manifold2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/2d/Solver2D.h

    ${LIBRARY_HEADERS_DIRECTORY}/2d/levels/ObjectLevel2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/2d/levels/PhysicsLevel2D.h

    # Sources
    ${LIBRARY_SOURCE_DIRECTORY}/2d/Transform2D.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/2d/Collider2D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/2d/CollisionUtils2D.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/2d/objects/Object2D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/2d/objects/RigidObject2D.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/2d/Solver2D.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/2d/levels/ObjectLevel2D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/2d/levels/PhysicsLevel2D.cpp
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