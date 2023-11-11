/* A prime example: Displays a blue window on startup. */
/*
#include <Icosahedron/Core.h>

class FirstProject : public ic::Application {
    public:
        bool init() override {
            // Use this to set up window settings, although this can also be done in the constructor
            displayName = "Example window";

            return true;
        }
        
        bool load() override {
            // This function is called after init() and after setting up the window

            return true;
        }

        void window_size_changed(int w, int h) override {
            // Called when the window is resized, and also if it is changed to fullscreen mode
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            // Called when events are retrieved and polled 

            return true;
        }

        bool update(float dt) override {
            // This is called once every frame
        
            clear_color(ic::Colors::blue);
            return true; 
        }

        void dispose() override {
            // Occurs when the application has to close

        }
};

int main(int argc, char *argv[]) {
    FirstProject application;

    // Constructs a window that is 640 pixels wide and 480 pixels tall
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/


#include <Icosahedron/Core.h>

class Example : public ic::Application {

    ic::Shader *testShader;
    ic::Mesh2D *testMesh;
    ic::Mesh2D *testSquare;
    ic::Camera2D *camera;
    
    ic::Texture *squareTexture;

    public:
        bool init() override {
            displayName = "Example window";
            scaling = ic::WindowScaling::resizeable;
            
            return true;
        }
        
        bool load() override {
            squareTexture = ic::TextureLoader::get().load_png("resources/textures/sand.png");

            testShader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D);
            testMesh = ic::GeometryGenerator::get().generate_regular_polygon_mesh(7, 0.3f);
            testMesh->set_transformation(ic::Mat4x4().set_translation<2>({ -0.5f, 0.0f }));

            std::vector<ic::Color> colors = {
                ic::Colors::red, 
                ic::Colors::green,
                ic::Colors::blue,
                ic::Colors::white
            };

            testSquare = ic::GeometryGenerator::get().generate_rectangle_mesh(0.2f, 0.2f);
            testSquare->add_attribute("colors", 1, 3, colors);
            testSquare->set_material(ic::MeshMaterial2D(ic::Colors::white, 0.1f));

            testSquare->set_transformation(ic::Mat4x4().set_translation<2>({ 0.5f, 0.0f }));

            camera = new ic::Camera2D();

            return true;
        }

        bool update(float dt) override {
            clear_color(ic::Colors::blue);

            testShader->use();
            camera->use(testShader);
            testMesh->draw(testShader);

            squareTexture->use();
            testSquare->draw(testShader);
            squareTexture->unuse();

            return true; 
        }

        void dispose() override {
            testShader->clear();

            squareTexture->dispose();
            
            testMesh->dispose();
            testSquare->dispose();
        }
};

int main(int argc, char *argv[]) {
    Example application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}