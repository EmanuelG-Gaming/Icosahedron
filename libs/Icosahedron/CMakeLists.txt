cmake_minimum_required(VERSION 3.0.0...3.13)

set(PROJECT_NAME Icosahedron)
set(LIBRARY_NAME Icosahedron)

set(LIBRARY_HEADERS_DIRECTORY include/${LIBRARY_NAME})

set(LIBRARY_HEADERS 
    ${LIBRARY_HEADERS_DIRECTORY}/Global.h
    ${LIBRARY_HEADERS_DIRECTORY}/Debug.h
    ${LIBRARY_HEADERS_DIRECTORY}/util/Time.h
    ${LIBRARY_HEADERS_DIRECTORY}/util/IO.h

    ${LIBRARY_HEADERS_DIRECTORY}/entity/ECS.h
    ${LIBRARY_HEADERS_DIRECTORY}/entity/Component.h
    ${LIBRARY_HEADERS_DIRECTORY}/entity/Entity.h
    ${LIBRARY_HEADERS_DIRECTORY}/entity/Entities.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/scene/SceneGraph.h


    ${LIBRARY_HEADERS_DIRECTORY}/graphics/Color.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Vector.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Vectors.h

    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Raycast.h

    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Spring2D.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Spring3D.h

    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Rectangle.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Polygon.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Circle.h

    ${LIBRARY_HEADERS_DIRECTORY}/math/Matrix.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/Matrices.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/Interpolation.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/EarClippingTriangulation.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Quaternion.h
    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Bezier.h

    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Rasterization.h

    ${LIBRARY_HEADERS_DIRECTORY}/math/geom/Quadtree.h


    ${LIBRARY_HEADERS_DIRECTORY}/graphics/Colors.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/Image.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/ImageIO.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/Images.h

    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/GL.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Shader.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Shaders.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Framebuffer.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Texture.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/TextureArray.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Cubemap.h

    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/TextureAtlas.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/VertexArray.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/gl/Batch.h

    ${LIBRARY_HEADERS_DIRECTORY}/graphics/TextAtlas.h
    ${LIBRARY_HEADERS_DIRECTORY}/graphics/FreeType.h

    ${LIBRARY_HEADERS_DIRECTORY}/util/Noise.h
    ${LIBRARY_HEADERS_DIRECTORY}/util/GeometryGenerator.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/Renderer.h

    ${LIBRARY_HEADERS_DIRECTORY}/input/Input.h
    ${LIBRARY_HEADERS_DIRECTORY}/input/InputHandler.h
    ${LIBRARY_HEADERS_DIRECTORY}/input/KeyboardController.h
    ${LIBRARY_HEADERS_DIRECTORY}/input/MouseController.h

    ${LIBRARY_HEADERS_DIRECTORY}/assets/loaders/ShaderLoader.h
    ${LIBRARY_HEADERS_DIRECTORY}/assets/loaders/TextureLoader.h
    ${LIBRARY_HEADERS_DIRECTORY}/assets/loaders/TextureAtlasLoader.h
    ${LIBRARY_HEADERS_DIRECTORY}/assets/loaders/CubemapLoader.h
    ${LIBRARY_HEADERS_DIRECTORY}/assets/loaders/OBJLoader.h
    ${LIBRARY_HEADERS_DIRECTORY}/assets/loaders/AssimpModelLoader.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/audio/AudioSource.h
    ${LIBRARY_HEADERS_DIRECTORY}/audio/Music.h
    ${LIBRARY_HEADERS_DIRECTORY}/audio/Sound.h
    ${LIBRARY_HEADERS_DIRECTORY}/audio/Audio.h

    ${LIBRARY_HEADERS_DIRECTORY}/scene/Mesh.h
    ${LIBRARY_HEADERS_DIRECTORY}/scene/Model.h
    ${LIBRARY_HEADERS_DIRECTORY}/scene/2d/Camera2D.h

    # ${LIBRARY_HEADERS_DIRECTORY}/scene/3d/Mesh3D.h
    ${LIBRARY_HEADERS_DIRECTORY}/scene/3d/Camera3D.h
    ${LIBRARY_HEADERS_DIRECTORY}/scene/3d/Skybox.h
    
    ${LIBRARY_HEADERS_DIRECTORY}/scene/3d/controllers/FreeRoamCameraController3D.h
    ${LIBRARY_HEADERS_DIRECTORY}/scene/3d/controllers/OrbitalCameraController3D.h

    ${LIBRARY_HEADERS_DIRECTORY}/Application.h
)

set(LIBRARY_SOURCE_DIRECTORY src)
set(LIBRARY_SOURCE
    ${LIBRARY_SOURCE_DIRECTORY}/util/Time.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/util/IO.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/Debug.cpp


    ${LIBRARY_SOURCE_DIRECTORY}/math/Mathf.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/math/Interpolation.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/math/geom/Bezier.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/math/geom/Raycast.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/math/geom/Rasterization.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/math/EarClippingTriangulation.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/graphics/Image.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/ImageIO.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/Images.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/GL.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/Shader.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/Framebuffer.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/Texture.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/Cubemap.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/TextureArray.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/VertexArray.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/gl/Batch.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/graphics/TextAtlas.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/graphics/FreeType.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/input/KeyboardController.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/input/MouseController.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/input/InputHandler.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/assets/loaders/ShaderLoader.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/assets/loaders/TextureLoader.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/assets/loaders/CubemapLoader.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/assets/loaders/OBJLoader.cpp
    #${LIBRARY_SOURCE_DIRECTORY}/assets/loaders/AssimpModelLoader.cpp
    
    ${LIBRARY_SOURCE_DIRECTORY}/audio/Music.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/audio/Sound.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/audio/Audio.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/scene/Mesh.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/scene/Model.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/scene/2d/Camera2D.cpp


    ${LIBRARY_SOURCE_DIRECTORY}/scene/3d/Camera3D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/scene/3d/Skybox.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/scene/3d/controllers/FreeRoamCameraController3D.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/scene/3d/controllers/OrbitalCameraController3D.cpp

    ${LIBRARY_SOURCE_DIRECTORY}/util/Noise.cpp
    ${LIBRARY_SOURCE_DIRECTORY}/util/GeometryGenerator.cpp

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


target_link_libraries(${LIBRARY_NAME} glad ${SDL2_LIBRARIES} soloud stb)