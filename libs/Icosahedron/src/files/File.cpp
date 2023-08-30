#include <Icosahedron/files/File.h>
#include <iostream>

using namespace ic;

File::File(const std::string &relativePath) {
    std::filesystem::path path = std::filesystem::current_path();
    handler = path.append(relativePath);
}

std::string ic::File::get_path() {
    std::string name = handler.u8string();
    std::replace(name.begin(), name.end(), '\\', '/');

    return name;
}

std::string ic::File::get_extension() {
    return handler.extension().u8string();
}