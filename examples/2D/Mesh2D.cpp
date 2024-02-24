#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>


/* Polygon example. Demonstrates the use of per-vertex colors, texturing, matrix transformations, and materials, all in the same shader program. */
class Mesh2D : public ic::Application {
    ic::Mesh2D mesh1, mesh2;

    ic::Texture texture;
    ic::Camera2D camera;

    ic::Shader shader;
    float time;

    public:
        bool init() override {
            displayName = "Mesh2D Example";
            
            return true;
        }
        
        bool load() override {
            // Mesh 1
            mesh1 = ic::GeometryGenerator::generate_regular_polygon_mesh(7, 0.3f);


            // Note that this can also be more verbosely expressed as:

            //auto vertices = ic::GeometryGenerator::generate_regular_polygon(7, 0.3f);
            //mesh1 = ic::Mesh2D();
            //mesh1.add_attribute(0, 2, vertices);
            //mesh1.add_attribute(2, 2, ic::GeometryGenerator::generate_UV_polygon(vertices));
            //mesh1.set_index_buffer(ic::EarClippingTriangulation::triangulate(vertices));
            
            // Mesh 2
            mesh2 = ic::GeometryGenerator::generate_regular_polygon_mesh(3, 0.3f);
            mesh2.add_attribute(1, 3, { ic::Colors::red, ic::Colors::green, ic::Colors::blue }); // Add a "color" attribute to mesh2

            // Add a material that slightly brightens the colours shown by the vertex attributes 
            mesh2.set_material(ic::MeshMaterial2D(ic::Colors::white, 0.2f));
            mesh2.set_transformation(ic::Mat4x4().set_translation<2>({ -0.35f, 0.0f }));
            

            shader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex2D, ic::Shaders::meshShaderFrag2D);
            texture = ic::TextureLoader::load_png("resources/textures/wood.png");
            
            camera = ic::Camera2D();

            time = 0.0f;

            return true;
        }

        void window_size_changed(int w, int h) override {
            camera.width = w;
            camera.height = h;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            time += dt;

            clear_color(ic::Colors::blue);
            
            ic::Mat4x4 combined, scaling, rotation, translation;
            scaling.set_scaling<2>({ ic::Mathf::sinf(time), ic::Mathf::cosf(time) });
            rotation.set_rotation(-time);
            translation.set_translation<2>({ 0.35f, 0.0f });
            
            // Multiplying matrices together this way follows column-major notation, so
            // translation * rotation * scaling is not the same as scaling * rotation * translation,
            // even though the latter seems to be a logical approach (i.e. you scale, then rotate, then translate)
            combined = translation * rotation * scaling;
            mesh1.set_transformation(combined);
            
            shader.use();
            camera.use(shader);


            // Draws a textured mesh
            texture.use();
            mesh1.draw(shader);
            texture.unuse();

            // Draws an untextured mesh with vertex colours
            mesh2.draw(shader);

            return true; 
        }

        void dispose() {
            shader.clear();
            
            texture.dispose();
            
            mesh1.dispose();
            mesh2.dispose();
        }
};

int main() {
    Mesh2D application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}