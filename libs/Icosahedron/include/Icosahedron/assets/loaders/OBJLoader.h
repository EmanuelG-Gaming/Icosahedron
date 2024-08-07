#ifndef IC_OBJ_LOADER_H
#define IC_OBJ_LOADER_H

#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <fstream>
#include <sstream>

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/scene/Mesh.h>

namespace ic {
    struct OBJMaterialInfo {
        ic::Vec3 ambient;
        ic::Vec3 diffuse;
        ic::Vec3 specular;

        float shininess = 0.0f;
    };


    struct OBJSizes {
        int posSize;
        int tCoordSize;
        int normalSize;
        int faceSize;
    };

    struct OBJAttributeReferences {
        std::vector<ic::Vec3f> posReference;
        std::vector<ic::Vec2f> tCoordReference;
        std::vector<ic::Vec3f> normalReference;
    
        std::vector<std::string> points;
    };

    struct OBJGeometricData {
        std::vector<ic::Vec3f> vertices;
        std::vector<ic::Vec2f> textureCoords;
        std::vector<ic::Vec3f> normals;

        std::vector<unsigned int> indices;
    };


    /** @brief Loads .obj model files. Can optionally load material content. 
     *  @deprecated Might get deprecated in favour of Assimp.
    */
    namespace OBJLoader {
        ic::Mesh load(const char *objectFileName);
        std::vector<ic::Mesh> load_multiple(const char *objectFileName, const char *separator = "o");

        std::map<std::string, ic::OBJMaterialInfo> get_materials(const char *materialFileName);
        
        namespace {
            ic::OBJSizes calculate_size(std::ifstream &objRead);
            ic::OBJAttributeReferences get_attribute_references(std::ifstream &objRead, const ic::OBJSizes &sizes);
            ic::OBJGeometricData get_geometric_data(const ic::OBJAttributeReferences &reference);
        };
    };
}
#endif