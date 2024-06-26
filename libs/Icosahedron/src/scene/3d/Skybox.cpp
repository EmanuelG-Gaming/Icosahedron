#include <Icosahedron/scene/3d/Skybox.h>
#include <stdexcept>

using namespace ic;


Skybox::Skybox() {
}

Skybox::Skybox(const std::vector<std::string> &fileNames, bool loadsBMP) {
    if (loadsBMP) {
        this->textureCube = ic::CubemapLoader::load_bmp(fileNames);
    } else {
        this->textureCube = ic::CubemapLoader::load_png(fileNames);
    }

    this->mesh = ic::Mesh();
    this->mesh.add_attribute(0, 3, ic::GeometryGenerator::generate_cube(1.0f));
    this->mesh.set_index_buffer(ic::GeometryGenerator::generate_parallelipiped_indices());
}

Skybox::Skybox(const ic::Mesh &mesh, const std::vector<std::string> &fileNames, bool loadsBMP) {
    if (loadsBMP) {
        this->textureCube = ic::CubemapLoader::load_bmp(fileNames);
    } else {
        this->textureCube = ic::CubemapLoader::load_png(fileNames);
    }

    this->mesh = mesh;
}



void ic::Skybox::draw(ic::GLPrimitives primitive) {
    glCullFace(GL_BACK);
    glDepthMask(GL_FALSE);

    this->textureCube.use();
    this->mesh.draw(primitive);

    glDepthMask(GL_TRUE);
    glCullFace(GL_FRONT);
}

void ic::Skybox::dispose() {
    this->textureCube.dispose();
    this->mesh.dispose();
}


void ic::Skybox::use_texture(int index) {
    this->textureCube.use(index);
}