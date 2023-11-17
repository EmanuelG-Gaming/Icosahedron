#include <Icosahedron/scene/3d/Skybox.h>
#include <stdexcept>

using namespace ic;


Skybox::Skybox(const std::vector<std::string> &fileNames) {
    this->textureCube = ic::CubemapLoader::get().load_png(fileNames);
    this->mesh = new ic::Mesh3D();

    this->mesh->add_attribute(0, 3, ic::GeometryGenerator::get().generate_cube(1.0f));
    this->mesh->set_index_buffer(ic::GeometryGenerator::get().generate_parallelipiped_indices());
}

Skybox::Skybox(ic::Mesh3D *mesh, const std::vector<std::string> &fileNames) {
    if (mesh == nullptr || mesh == NULL) {
        throw std::invalid_argument("Skybox mesh can't be null!");
    }

    this->textureCube = ic::CubemapLoader::get().load_png(fileNames);
    this->mesh = mesh;
}



void ic::Skybox::draw(ic::Shader *shader, ic::GLPrimitives primitive) {
    glCullFace(GL_BACK);
    glDepthMask(GL_FALSE);

    this->textureCube->use();
    this->mesh->draw(shader, primitive);

    glDepthMask(GL_TRUE);
    glCullFace(GL_FRONT);
}

void ic::Skybox::dispose() {
    this->textureCube->dispose();
    this->mesh->dispose();
}


void ic::Skybox::use_texture(int index) {
    this->textureCube->use(index);
}