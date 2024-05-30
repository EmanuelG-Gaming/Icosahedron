#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/Mesh.h>
#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>

#include <Icosahedron/scene/SceneGraph.h>
#include <Icosahedron/Debug.h>

class MeshNode : public ic::SceneGraphNode {
    public:
        ic::Vec2 pos;
        ic::Vec2 scaling = { 1.0f, 1.0f };
        float rotation = 0.0f;

        ic::Mat4x4 transform;
        ic::Mesh mesh;


        MeshNode() {
        }

        void update_recursively() {
            if (this->previous) {
                MeshNode *p = dynamic_cast<MeshNode*>(this->previous);
                if (p != nullptr) {
                    this->transform = p->transform * this->get_local_model_matrix();   
                } 
            } else {
                this->transform = this->get_local_model_matrix();
            }


            for (auto &elem : this->nodes) {
                MeshNode *m = dynamic_cast<MeshNode*>(elem);
                if (m == nullptr) continue;

                m->update_recursively();
            }
        }
        
        void render_recursively(ic::Shader &shader) {
            this->mesh.set_transformation(this->transform);
            this->mesh.apply_transformations(shader);
            this->mesh.draw();


            for (auto &elem : this->nodes) {
                MeshNode *m = dynamic_cast<MeshNode*>(elem);
                if (m == nullptr) continue;

                m->render_recursively(shader);
            }
        }

        void dispose_recursively() {
            
        }

        ic::Mat4x4 get_local_model_matrix() {
            ic::Mat4x4 translation = ic::Mat4x4().set_translation<2>(pos);
            ic::Mat4x4 scale = ic::Mat4x4().set_scaling<2>(scaling);
            ic::Mat4x4 rotate = ic::Mat4x4().set_rotation(rotation);

            ic::Mat4x4 result = translation * rotate * scale;

            return result;
        }
};


MeshNode *rootNode;

ic::Texture texture;
ic::Camera2D camera;
ic::Shader shader;


void game_load() {
    rootNode = new MeshNode();

    {
        ic::Mesh m = ic::GeometryGenerator::generate_regular_polygon_mesh(7, 0.3f);

        MeshNode *node = new MeshNode();
        node->mesh = m;
        node->pos = { -0.3f, 0.0f };

        rootNode->add_node(node);

        {
            ic::Mesh m2 = ic::GeometryGenerator::generate_regular_polygon_mesh(3, 0.1f);

            MeshNode *node2 = new MeshNode();
            node2->mesh = m2;
            node2->pos = { 0.0f, 0.5f };
    
            node->add_node(node2);


            node->remove_all();
        }
    }
    
    shader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex2D, ic::Shaders::meshShaderFrag2D);
    texture = ic::TextureLoader::load_png("resources/textures/wood.png");
    
    camera = ic::Camera2D();
}



void game_update() {
    rootNode->update_recursively();

    ic::GL::clear_color(ic::Colors::blue);
    shader.use();
    camera.use(shader);

    texture.use();
    rootNode->render_recursively(shader);
    texture.unuse();
}

void game_dispose() {
    shader.clear();
            
    texture.dispose();
    
    rootNode->dispose_recursively();
}


int main() {
    ic::create_console();

    ic::Engine engine;

    engine.construct_window();
    engine.create_GL_context();

    engine.window.set("Scene Graph", 640, 480);
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