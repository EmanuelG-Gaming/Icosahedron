#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/graphics/gl/Shaders.h>

#include <UI/Core.h>


void ic::UI::Core::load() {
    std::string vertex = IC_ADD_GLSL_DEFINITION(
        layout (location = 0) in vec2 position;
        layout (location = 1) in vec3 color;
        layout (location = 2) in vec2 tCoords;

        uniform mat4 projection = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
        
        out vec3 vColor;
        out vec2 vTCoords;

        void main() {
            vColor = color;
            vTCoords = tCoords;
            vec4 pos = vec4(position, 0.0, 1.0);

            pos = projection * pos;
            
            gl_Position = pos;
        }
    );

    std::string fragment = IC_ADD_GLSL_DEFINITION(
        precision mediump float;

        in vec3 vColor;
        in vec2 vTCoords;

        uniform sampler2D sampleTexture;

        out vec4 outColor;

        void main() {
            vec4 color = texture(sampleTexture, vTCoords);
            if (color.r <= 0.1) discard;
            outColor = vec4(color.r, color.r, color.r, 1.0) * vec4(vColor, 1.0);
        }
    );



    ic::UI::Global::get().load();

    this->uiCamera = new ic::Camera2D();
    this->uiShader = ic::ShaderLoader::get().load(vertex, fragment);

    this->mainTable = new ic::UI::Table();
}

void ic::UI::Core::render() {
    this->uiShader->use();
    this->uiCamera->use(this->uiShader);
    
    this->mainTable->draw();

    ic::UI::Global::get().fillBatch->render();
}


void ic::UI::Core::dispose() {
    this->uiShader->clear();
    ic::UI::Global::get().fillBatch->dispose();
}