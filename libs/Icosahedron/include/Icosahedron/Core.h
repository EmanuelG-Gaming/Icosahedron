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
#include <Icosahedron/math/geom/Vector.h>
#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/math/geom/Rectangle.h>

#include <Icosahedron/math/Matrix.h>
#include <Icosahedron/math/Matrices.h>

// Graphics
#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/gl/Batch.h>

// Input
#include <Icosahedron/input/Input.h>
#include <Icosahedron/input/KeyboardController.h>
#include <Icosahedron/input/WASDController.h>
#include <Icosahedron/input/InputHandler.h>

// Scene
#include <Icosahedron/scene/2d/Shape2D.h>
#include <Icosahedron/scene/2d/RectangleShape.h>

// App
#include <Icosahedron/Application.h>


#endif