#ifndef IC_OBJ_LOADER_H
#define IC_OBJ_LOADER_H

#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <fstream>
#include <sstream>

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/scene/3d/Mesh3D.h>

namespace ic {
    struct OBJMaterialInfo {
        ic::Vec3f ambient = { 0.0f, 0.0f, 0.0f };
        ic::Vec3f diffuse = { 0.0f, 0.0f, 0.0f };
        ic::Vec3f specular = { 0.0f, 0.0f, 0.0f };

        float shininess = 0.0f;
    };

    /** @brief Loads .obj model files. Can optionally load material content. */
    class OBJLoader {
        public:
            static OBJLoader& get() {
                static OBJLoader ins;
                return ins;
            }

            ic::Mesh3D *get_mesh(const std::string &objectFileName);

            std::map<std::string, ic::OBJMaterialInfo> get_materials(const std::string &materialFileName);

        private:
            OBJLoader() {}
            ~OBJLoader() {}

        public:
            OBJLoader(OBJLoader const&) = delete;
            void operator = (OBJLoader const&) = delete;
    };
}
#endif