#ifndef IC_SHADER_LOADER_H
#define IC_SHADER_LOADER_H

#include <string>
#include <fstream>
#include <sstream>

#include <Icosahedron/graphics/gl/Shader.h>


namespace ic {
    class ShaderLoader {
        public:
            static ShaderLoader& get() {
                static ShaderLoader ins;
                return ins;
            }

            /** @brief Loads a shader from raw code. This does not load from a file. */
            ic::Shader load(const std::string &vertexCode, const std::string &fragmentCode);

            ic::Shader load_file(const std::string &vertexFileName, const std::string &fragmentFileName);

        private:
            ShaderLoader() {}
            ~ShaderLoader() {}

        public:
            ShaderLoader(ShaderLoader const&) = delete;
            void operator = (ShaderLoader const&) = delete;
    };
}

#endif