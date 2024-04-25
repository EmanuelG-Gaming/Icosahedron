cmake_minimum_required(VERSION 3.0.0...3.13)

set(PROJECT_NAME Physics)
set(LIBRARY_NAME Physics)

set(LIBRARY_HEADERS_DIRECTORY include/${LIBRARY_NAME})
set(LIBRARY_SOURCE_DIRECTORY src)



project(${PROJECT_NAME})

add_library(${LIBRARY_NAME} STATIC
    # -----Headers-----
    # 3D
    ${LIBRARY_HEADERS_DIRECTORY}/Transform.h

    ${LIBRARY_HEADERS_DIRECTORY}/ManifoldPoints.h

    ${LIBRARY_HEADERS_DIRECTORY}/Collider.h
    ${LIBRARY_HEADERS_DIRECTORY}/CollisionUtils.h

    ${LIBRARY_HEADERS_DIRECTORY}/objects/Object.h
    ${LIBRARY_HEADERS_DIRECTORY}/Manifold.h
    ${LIBRARY_HEADERS_DIRECTORY}/objects/RigidObject.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/Solver.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/forces/Force.h
    ${LIBRARY_HEADERS_DIRECTORY}/forces/Gravity.h
    ${LIBRARY_HEADERS_DIRECTORY}/forces/WindForce.h

    ${LIBRARY_HEADERS_DIRECTORY}/levels/PhysicsLevel.h



    # -----Sources-----
    # 3D
    ${LIBRARY_SOURCE_DIRECTORY}/Transform.cpp
    
    ${LIBRARY_SOURCE_DIRECTORY}/Collider.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/CollisionUtils.cpp
    
    ${LIBRARY_SOURCE_DIRECTORY}/objects/Object.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/objects/RigidObject.cpp
    
    ${LIBRARY_SOURCE_DIRECTORY}/Solver.cpp
    
    ${LIBRARY_SOURCE_DIRECTORY}/forces/Gravity.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/forces/WindForce.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/levels/PhysicsLevel.cpp
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