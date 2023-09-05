#ifndef IC_CORE_H
#define IC_CORE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

/* -----Icosahedron----- */
namespace ic {
}

// Math
#include <Icosahedron/math/Mathf.h>

#include <Icosahedron/math/geom/Vector.h>
#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/math/geom/Rectangle.h>
#include <Icosahedron/math/geom/Polygon.h>
#include <Icosahedron/math/EarClippingTriangulation.h>

#include <Icosahedron/math/Matrix.h>
#include <Icosahedron/math/Matrices.h>
#include <Icosahedron/math/Interpolation.h>

#include <Icosahedron/math/geom/Quaternion.h>

// Files
#include <Icosahedron/files/File.h>

// Graphics
#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/Image.h>
#include <Icosahedron/graphics/ImageIO.h>

#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/GLStateHandler.h>

#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Framebuffer.h>
#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/gl/TextureAtlas.h>
#include <Icosahedron/graphics/gl/Batch.h>
#include <Icosahedron/graphics/gl/VertexArray.h>

#include <Icosahedron/graphics/TextAtlas.h>
#include <Icosahedron/graphics/FreeType.h>

// Input
#include <Icosahedron/input/Input.h>
#include <Icosahedron/input/KeyboardController.h>
#include <Icosahedron/input/MouseController.h>
#include <Icosahedron/input/InputHandler.h>

// Scene
#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/scene/3d/Mesh3D.h>
#include <Icosahedron/scene/3d/Camera3D.h>
#include <Icosahedron/scene/3d/controllers/FreeRoamCameraController3D.h>
#include <Icosahedron/scene/3d/controllers/OrbitalCameraController3D.h>

// Utilities
#include <Icosahedron/util/GeometryGenerator.h>

// Assets
#include <Icosahedron/assets/loaders/TextureLoader.h>
#include <Icosahedron/assets/loaders/OBJLoader.h>

// Global
#include <Icosahedron/Global.h>

// Renderer
#include <Icosahedron/Renderer.h>

// App
#include <Icosahedron/Application.h>


#endif