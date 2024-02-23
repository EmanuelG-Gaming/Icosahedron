#ifndef IC_SHADER_LOADER_H
#define IC_SHADER_LOADER_H

#include <string>
#include <fstream>
#include <sstream>

#include <Icosahedron/graphics/gl/Shader.h>


namespace ic {
    namespace ShaderLoader {
        /** @brief Loads a shader from raw code. This does not load from a file. */
        ic::Shader load(const std::string &vertexCode, const std::string &fragmentCode);
        ic::Shader load_file(const std::string &vertexFileName, const std::string &fragmentFileName);
    };
}

#endif