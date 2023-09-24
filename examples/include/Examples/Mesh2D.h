#ifndef IC_EXAMPLE_MESH_2D_H
#define IC_EXAMPLE_MESH_2D_H

/* Polygon example. Demonstrates the use of per-vertex colors, separate texturing (no atlas), matrix transformations, and materials, all in the same shader program. */
#include <Icosahedron/Core.h>

class Mesh2D : public ic::Application {
    ic::Mesh2D *mesh1;
    ic::Mesh2D *mesh2;

    ic::Texture<ic::T2D> *texture, *whiteTexture;
    ic::Camera2D *camera;

    ic::Shader *shader;
    float time;

    public:
        bool init() override {
            displayName = "Example window";
            scaling = ic::WindowScaling::fullscreen;

            return true;
        }
        
        bool load() override {
            // Mesh 1
            std::vector<float> positions = ic::GeometryGenerator::get().generate_regular_polygon(7, 0.3f);
            std::vector<float> textureCoords = ic::GeometryGenerator::get().generate_UV_polygon(positions);
            std::vector<unsigned int> indices = ic::EarClippingTriangulation::get().triangulate(positions);
            
            mesh1 = new ic::Mesh2D(positions);
            // Jump past the color attribute
            mesh1->jump_attribute();
            mesh1->add_attribute("textureCoords", 2, textureCoords);
            mesh1->set_index_buffer(indices);
            mesh1->set_material(ic::MeshMaterial2D(ic::Colors::white, 1.0f));
            
            // Mesh 2
            std::vector<float> triangle = ic::GeometryGenerator::get().generate_regular_polygon(3, 0.3f);
            
            mesh2 = new ic::Mesh2D(triangle);
            mesh2->add_attribute("color", 3, { ic::Colors::red, ic::Colors::green, ic::Colors::blue });
            mesh2->set_index_buffer({ 0, 1, 2 });
            mesh2->set_material(ic::MeshMaterial2D(ic::Colors::white, 0.2f));
            mesh2->set_transformation(ic::Mat4x4().set_translation<2>({ -0.35f, 0.0f }));


            shader = new ic::Shader(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D, false);
            
            texture = new ic::Texture<ic::T2D>("resources/textures/wood.png");
            whiteTexture = new ic::Texture<ic::T2D>("resources/textures/white.png");

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
            rotation.set_rotation(time);
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

#endif