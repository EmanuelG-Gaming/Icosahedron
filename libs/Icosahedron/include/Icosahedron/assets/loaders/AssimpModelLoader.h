#ifndef IC_ASSIMP_MODEL_LOADER_H
#define IC_ASSIMP_MODEL_LOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <Icosahedron/scene/Model.h>

namespace ic {
    /** @brief Uses Assimp to load a set of meshes, and optionally texture materials. */
    namespace AssimpModelLoader {
        ic::Model load_model(const char *filePath);
        ic::Model load_model(const std::string &filePath);
        
    };
}
#endif