#ifndef IC_MODEL_H
#define IC_MODEL_H

#include <vector>

#include <Icosahedron/scene/Mesh.h>


namespace ic {
    class Model {
        public:
            std::vector<ic::Mesh> meshes;

            void draw(ic::Shader &shader, ic::GLPrimitives primitive = ic::TRIANGLES);
    };
}
#endif