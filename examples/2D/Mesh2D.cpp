#include <Icosahedron/Core.h>

/* Polygon example. Demonstrates the use of per-vertex colors, separate texturing (no atlas), matrix transformations, and materials, all in the same shader program. */
class Mesh2D : public ic::Application {
    ic::Mesh2D *mesh1;
    ic::Mesh2D *mesh2;

    ic::Texture *texture, *whiteTexture;
    ic::Camera2D *camera;

    ic::Shader *shader;
    float time;

    public:
        bool init() override {
            displayName = "Mesh2D Example";
            scaling = ic::WindowScaling::fullscreen;

            return true;
        }
        
        bool load() override {
            // Mesh 1
            mesh1 = ic::GeometryGenerator::get().generate_regular_polygon_mesh(7, 0.3f);

            // Note that this can also be more verbosely expressed as
            //mesh1 = new ic::Mesh2D();
            //mesh1->add_attribute("position", 0, 2, positions);
            //mesh1->add_attribute("textureCoords", 2, 2, textureCoords);
            //mesh1->set_index_buffer(indices);

            mesh1->set_material(ic::MeshMaterial2D(ic::Colors::white, 1.0f));
            
            
            // Mesh 2
            ic::Vec2f pos;
            mesh2 = ic::GeometryGenerator::get().generate_regular_polygon_mesh(3, 0.3f);
            mesh2->add_attribute("color", 1, 3, { ic::Colors::red, ic::Colors::green, ic::Colors::blue });

            mesh2->set_material(ic::MeshMaterial2D(ic::Colors::white, 0.2f));
            //mesh2->set_transformation(ic::Mat4x4().set_translation<2>({ -0.35f, 0.0f }));
            mesh2->set_transformation(ic::Mat4x4().set_translation<2>(pos));

            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D);
            
            texture = ic::TextureLoader::get().load_png("resources/textures/wood.png");
            whiteTexture = ic::TextureLoader::get().load_png("resources/textures/white.png");

            camera = new ic::Camera2D();

            time = 0.0f;

            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            time += dt;

            clear_color(ic::Colors::blue);
            
            ic::Mat4x4 combined, scaling, rotation, translation;
            scaling.set_scaling<2>({ ic::Mathf::get().sinf(time), ic::Mathf::get().cosf(time) });
            rotation.set_rotation(-time);
            translation.set_translation<2>({ 0.35f, 0.0f });
            
            // Multiplying matrices together this way follows column-major notation, so
            // translation * rotation * scaling is not the same as scaling * rotation * translation,
            // even though the latter seems to be a logical approach (i.e. you scale, then rotate, then translate)
            combined = translation * rotation * scaling;
            mesh1->set_transformation(combined);
            
            shader->use();
            camera->use(shader);

            texture->use();
            mesh1->draw(shader);
            whiteTexture->use();
            mesh2->draw(shader);

            return true; 
        }

        void dispose() {
            shader->clear();
            
            texture->dispose();
            whiteTexture->dispose();

            mesh1->dispose();
            mesh2->dispose();
        }
};

int main(int argc, char *argv[]) {
    Mesh2D application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}