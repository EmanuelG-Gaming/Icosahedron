#ifndef IC_SKYBOX_H
#define IC_SKYBOX_H

#include <vector>

#include <glad/glad.h>

#include <Icosahedron/graphics/gl/GL.h>
#include <Icosahedron/graphics/gl/Cubemap.h>

#include <Icosahedron/assets/loaders/CubemapLoader.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/scene/Mesh.h>


namespace ic {
    class Skybox {
        public:
            /** @brief Initializes an empty skybox. */
            Skybox();

            /** @brief Initializes a cube skybox with face images. 
             *  @param fileNames File names (f1...f6) corresponding to each face.
             *  @note f1 - right face
             *  @note f2 - left face
             *  @note f3 - top face
             *  @note f4 - bottom face
             *  @note f5 - front face
             *  @note f6 - back face
            */
            Skybox(const std::vector<std::string> &fileNames, bool loadsBMP = false);

            /** @brief Initializes a new skybox, depending on a mesh. 
             *  @param fileNames File names (f1...f6) corresponding to each face.
             *  @note f1 - right face
             *  @note f2 - left face
             *  @note f3 - top face
             *  @note f4 - bottom face
             *  @note f5 - front face
             *  @note f6 - back face
            */
            Skybox(const ic::Mesh &mesh, const std::vector<std::string> &fileNames, bool loadsBMP = false);

            void draw(ic::GLPrimitives primitive = ic::TRIANGLES);
            void dispose();

            void use_texture(int index = 0);

        private:
            ic::Cubemap textureCube;
            ic::Mesh mesh;
    };
}
#endif