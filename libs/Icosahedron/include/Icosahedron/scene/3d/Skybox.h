#ifndef IC_SKYBOX_H
#define IC_SKYBOX_H

#include <vector>

#include <glad/glad.h>

#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Texture.h>

#include <Icosahedron/assets/loaders/CubemapLoader.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/scene/3d/Mesh3D.h>


namespace ic {
    class Skybox {
        public:
            /** @brief Initializes a cube skybox with face images. 
             *  @param fileNames File names (f1...f6) corresponding to each face.
             *  @note f1 - right face
             *  @note f2 - left face
             *  @note f3 - top face
             *  @note f4 - bottom face
             *  @note f5 - front face
             *  @note f6 - back face
            */
            Skybox(const std::vector<std::string> &fileNames);

            void draw(ic::Shader *shader, ic::GLPrimitives primitive = ic::TRIANGLES);
            void dispose();

            void use_texture(int index = 0);

        private:
            ic::Texture *textureCube;
            ic::Mesh3D *mesh;
    };
}
#endif