#include <Icosahedron/Core.h>
#include <Physics/2d/Transform2D.h>

/** @brief Icosahedron's physics engine. */
class BasicPhysics : public ic::Application {
    ic::Mesh2D *mesh1;
    ic::Camera2D *camera;
    ic::Shader *shader;
    float time;

    public:
        bool init() override {
            displayName = "Basic physics example";

            return true;
        }
        
        bool load() override {
            ic::Physics::Transform2D transform;
            transform.position = { 2.0f, 5.0f };
            transform.angle = 2.0f;

            mesh1 = ic::GeometryGenerator::get().generate_regular_polygon_mesh(5, 0.3f);
            mesh1->set_material(ic::MeshMaterial2D(ic::Colors::yellow, 1.0f));

            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D);
            
            camera = new ic::Camera2D();
            time = 0.0f;

            return true;
        }

        void window_size_changed(int w, int h) override {
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            time += dt;
            mesh1->set_transformation(ic::Mat4x4().set_translation<2>({ sin(time), 0.0f }));

            clear_color(ic::Colors::blue);

            shader->use();
            camera->use(shader);
            //mesh1->draw(shader, ic::LINE_STRIPS);
            mesh1->draw(shader);

            return true; 
        }

        void dispose() override {
            shader->clear();
            mesh1->dispose();
        }
};

int main(int argc, char *argv[]) {
    BasicPhysics application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}