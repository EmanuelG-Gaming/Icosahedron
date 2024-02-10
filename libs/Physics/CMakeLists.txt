cmake_minimum_required(VERSION 3.0.0...3.13)

set(PROJECT_NAME Physics)
set(LIBRARY_NAME Physics)

set(LIBRARY_HEADERS_DIRECTORY include/${LIBRARY_NAME})
set(LIBRARY_SOURCE_DIRECTORY src)



project(${PROJECT_NAME})

add_library(${LIBRARY_NAME} STATIC
    # -----Headers-----
    # 2D
    ${LIBRARY_HEADERS_DIRECTORY}/2d/Transform2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/2d/ManifoldPoints2D.h

    ${LIBRARY_HEADERS_DIRECTORY}/2d/Collider2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/2d/CollisionUtils2D.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/2d/objects/Object2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/2d/objects/RigidObject2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/2d/objects/SpringMassSystem2D.h

    ${LIBRARY_HEADERS_DIRECTORY}/2d/Manifold2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/2d/Solver2D.h

    ${LIBRARY_HEADERS_DIRECTORY}/2d/forces/Force2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/2d/forces/Gravity2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/2d/forces/WindForce2D.h

    ${LIBRARY_HEADERS_DIRECTORY}/2d/levels/PhysicsLevel2D.h

    # 3D
    ${LIBRARY_HEADERS_DIRECTORY}/3d/Transform3D.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/3d/ManifoldPoints3D.h
    ${LIBRARY_HEADERS_DIRECTORY}/3d/Collider3D.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/3d/CollisionUtils3D.h
    ${LIBRARY_HEADERS_DIRECTORY}/3d/objects/Object3D.h
    ${LIBRARY_HEADERS_DIRECTORY}/3d/objects/RigidObject3D.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/3d/Manifold3D.h
    ${LIBRARY_HEADERS_DIRECTORY}/3d/Solver3D.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/3d/forces/Force3D.h
    ${LIBRARY_HEADERS_DIRECTORY}/3d/forces/Gravity3D.h
    ${LIBRARY_HEADERS_DIRECTORY}/3d/forces/WindForce3D.h

    ${LIBRARY_HEADERS_DIRECTORY}/3d/levels/PhysicsLevel3D.h



    # -----Sources-----
    # 2D
    ${LIBRARY_SOURCE_DIRECTORY}/2d/Transform2D.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/2d/Collider2D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/2d/CollisionUtils2D.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/2d/objects/Object2D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/2d/objects/RigidObject2D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/2d/objects/SpringMassSystem2D.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/2d/Solver2D.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/2d/forces/Gravity2D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/2d/forces/WindForce2D.cpp


    ${LIBRARY_SOURCE_DIRECTORY}/2d/levels/PhysicsLevel2D.cpp

    # 3D
    ${LIBRARY_SOURCE_DIRECTORY}/3d/Transform3D.cpp
    
    ${LIBRARY_SOURCE_DIRECTORY}/3d/Collider3D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/3d/CollisionUtils3D.cpp
    
    ${LIBRARY_SOURCE_DIRECTORY}/3d/objects/Object3D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/3d/objects/RigidObject3D.cpp
    
    ${LIBRARY_SOURCE_DIRECTORY}/3d/Solver3D.cpp
    
    ${LIBRARY_SOURCE_DIRECTORY}/3d/forces/Gravity3D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/3d/forces/WindForce3D.cpp


    ${LIBRARY_SOURCE_DIRECTORY}/3d/levels/PhysicsLevel3D.cpp
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