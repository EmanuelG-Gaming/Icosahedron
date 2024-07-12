#include <Icosahedron/scene/Model.h>

void ic::Model::draw(ic::Shader &shader, ic::GLPrimitives primitive) {
    for (auto &mesh : this->meshes) {
        mesh.apply_transformations(shader);
        mesh.draw(primitive);
    }
}