#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/Mesh.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>

ic::Mesh mesh1, mesh2;
ic::Texture texture;
ic::Camera2D camera;
ic::Shader shader;


void game_load() {
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
    
    // Because the mesh only has 3 vertices, using glDrawArrays instead of 
    // the index buffer way of glDrawIndices will have no new visual changes at all
    mesh2.using_indices(false);
    mesh2.set_index_count(3);
    
    // Add a material that slightly brightens the colours shown by the vertex attributes 
    mesh2.set_transformation(ic::Mat4x4().set_translation<2>({ -0.35f, 0.0f }));
    
    shader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex2D, ic::Shaders::meshShaderFrag2D);
    texture = ic::TextureLoader::load("resources/textures/wood.png");
    
    camera = ic::Camera2D();
}


void game_update() {
    ic::GL::clear_color(ic::Colors::blue);
            
    ic::Mat4x4 combined, scaling, rotation, translation;
    scaling.set_scaling<2>({ ic::Mathf::sinf(ic::Time::time), ic::Mathf::cosf(ic::Time::time) });
    rotation.set_rotation(-ic::Time::time);
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
    mesh1.apply_transformations(shader);
    mesh1.draw();
    texture.unuse();

    // Draws an untextured mesh with vertex colours
    mesh2.apply_transformations(shader);
    mesh2.draw();
}

void game_dispose() {
    shader.clear();
            
    texture.dispose();
    
    mesh1.dispose();
    mesh2.dispose();
}


int main() {
    ic::Engine engine;

    engine.construct_window();
    engine.create_GL_context();

    engine.window.set("Mesh 2D", 640, 480);
    engine.window.set_vsync(1);
    

    game_load();

    bool enabled = true;
    ic::Event event;

    while (enabled) {
        while (engine.poll_events(event)) {
            if (!engine.process_window_callbacks(event)) {
                enabled = false;
                break;
            }
        }
        ic::InputHandler::update();

        // Updating every frame
        ic::GL::clear_color(ic::Colors::green);
        game_update();


        // --------------------

        engine.window.swap_buffers();
	    engine.tick();
    }

    game_dispose();

    return 0;
}