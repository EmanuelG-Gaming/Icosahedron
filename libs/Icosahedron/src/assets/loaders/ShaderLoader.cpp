#include <Icosahedron/assets/loaders/ShaderLoader.h>


ic::Shader ic::ShaderLoader::load(const std::string &vertexCode, const std::string &fragmentCode) {
    ic::Shader result(vertexCode, fragmentCode);
    return result;
}

ic::Shader ic::ShaderLoader::load_file(const std::string &vertexFileName, const std::string &fragmentFileName) {
    std::ifstream vert;
    std::ifstream frag;

    std::string vertexContent, fragmentContent;


    vert.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    frag.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vert.open(vertexFileName);
        frag.open(fragmentFileName);
                
        std::stringstream stringVert, stringFrag;
        stringVert << vert.rdbuf();
        stringFrag << frag.rdbuf();
                
        vert.close();
        frag.close();
                  
        vertexContent = stringVert.str();
        fragmentContent = stringFrag.str();
    } catch (std::ifstream::failure except) {
        printf("Couldn't open the shader file: %s\n", except.what());
    }

    ic::Shader result(vertexContent, fragmentContent);
    
    return result;
}