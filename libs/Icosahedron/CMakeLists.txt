cmake_minimum_required(VERSION 3.0.0)

set(PROJECT_NAME IcosahedronLibrary)
set(LIBRARY_NAME Icosahedron)

set(LIBRARY_HEADERS_DIRECTORY include/${LIBRARY_NAME})
set(LIBRARY_HEADERS 
    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Vector.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Vectors.h

    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Rectangle.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Polygon.h
    

    ${LIBRARY_HEADERS_DIRECTORY}/math/Matrix.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/Matrices.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/Interpolation.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/EarClippingTriangulation.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Quaternion.h

    ${LIBRARY_HEADERS_DIRECTORY}/graphics/Color.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/Colors.h

    ${LIBRARY_HEADERS_DIRECTORY}/files/File.h

    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/GL.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/GLStateHandler.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Shader.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Shaders.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Framebuffer.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Texture.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/TextureAtlas.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/VertexArray.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Batch.h

    ${LIBRARY_HEADERS_DIRECTORY}/graphics/TextAtlas.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/FreeType.h

    ${LIBRARY_HEADERS_DIRECTORY}/util/GeometryGenerator.h
    ${LIBRARY_HEADERS_DIRECTORY}/Renderer.h

    ${LIBRARY_HEADERS_DIRECTORY}/input/Input.h
    ${LIBRARY_HEADERS_DIRECTORY}/input/InputHandler.h
    ${LIBRARY_HEADERS_DIRECTORY}/input/KeyboardController.h
    ${LIBRARY_HEADERS_DIRECTORY}/input/MouseController.h

    ${LIBRARY_HEADERS_DIRECTORY}/scene/2d/Shape2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/scene/2d/RectangleShape.h
    ${LIBRARY_HEADERS_DIRECTORY}/scene/2d/PolygonShape.h
    ${LIBRARY_HEADERS_DIRECTORY}/scene/2d/Mesh2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/scene/2d/Camera2D.h

    ${LIBRARY_HEADERS_DIRECTORY}/scene/3d/Mesh3D.h
    ${LIBRARY_HEADERS_DIRECTORY}/scene/3d/Camera3D.h

    ${LIBRARY_HEADERS_DIRECTORY}/scene/3d/controllers/FreeRoamCameraController3D.h
    ${LIBRARY_HEADERS_DIRECTORY}/scene/3d/controllers/OrbitalCameraController3D.h

    ${LIBRARY_HEADERS_DIRECTORY}/assets/modeling/OBJLoader.h

    ${LIBRARY_HEADERS_DIRECTORY}/Global.h
    ${LIBRARY_HEADERS_DIRECTORY}/Application.h
    ${LIBRARY_HEADERS_DIRECTORY}/Core.h
)

set(LIBRARY_SOURCE_DIRECTORY src)
set(LIBRARY_SOURCE
    ${LIBRARY_SOURCE_DIRECTORY}/files/File.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/GLStateHandler.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/Shader.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/Framebuffer.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/VertexArray.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/Batch.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/graphics/TextAtlas.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/FreeType.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/input/KeyboardController.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/input/MouseController.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/input/InputHandler.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/scene/2d/RectangleShape.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/scene/2d/PolygonShape.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/scene/2d/Mesh2D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/scene/2d/Camera2D.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/scene/3d/Mesh3D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/scene/3d/Camera3D.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/scene/3d/controllers/FreeRoamCameraController3D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/scene/3d/controllers/OrbitalCameraController3D.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/assets/modeling/OBJLoader.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/Renderer.cpp
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