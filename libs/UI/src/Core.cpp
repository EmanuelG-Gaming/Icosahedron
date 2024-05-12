#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/input/InputHandler.h>

#include <UI/Global.h>
#include <UI/Core.h>
#include <UI/TextField.h>


void ic::UI::Core::load() {
    std::string vertex = IC_ADD_GLSL_DEFINITION(
        layout (location = 0) in vec2 position;
        layout (location = 1) in vec4 color;
        layout (location = 2) in vec2 tCoords;

        uniform mat4 projection = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
        
        out vec4 vColor;
        out vec2 vTCoords;

        void main() {
            vColor = color;
            vTCoords = tCoords;
            vec4 pos = vec4(position, 0.0, 1.0);

            pos = projection * pos;
            
            gl_Position = pos;
        }
    );


    std::string textureFragment = IC_ADD_GLSL_DEFINITION(
        precision mediump float;

        in vec4 vColor;
        in vec2 vTCoords;

        uniform sampler2D sampleTexture;

        out vec4 outColor;

        void main() {
            vec4 color = texture(sampleTexture, vTCoords);
            outColor = color * vColor;
        }
    );

    std::string textFragment = IC_ADD_GLSL_DEFINITION(
        precision mediump float;

        in vec4 vColor;
        in vec2 vTCoords;

        uniform sampler2D sampleTexture;

        out vec4 outColor;

        void main() {
            vec4 color = texture(sampleTexture, vTCoords);
            if (color.r <= 0.5) discard;
            outColor = vec4(color.r, color.r, color.r, 1.0) * vec4(vColor.rgb, 1.0);
        }
    );




    ic::UI::Global::get().load();

    this->uiCamera = ic::Camera2D();
    this->uiShader = ic::ShaderLoader::load(vertex, textureFragment);
    this->uiTextShader = ic::ShaderLoader::load(vertex, textFragment);
    this->atlas = &ic::UI::Global::get().atlas;

    this->mainTable = new ic::UI::Table();


    this->mouse = new ic::MouseController();
    this->mouse->add_mouse_moved_action([this]() {
        ic::Vec2i p = this->mouse->get_cursor_position();
        ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };

        ic::UI::Global::get().mouseCursorPosition = this->uiCamera.unproject(pos);

        this->mainTable->mouse_moved();
    });

    this->mouse->add_mouse_up_action([this]() {
        this->mainTable->mouse_up();

        if (ic::UI::Global::get().focusedTextField != nullptr && ic::UI::Global::get().focusedTextField->is_focused()) {
            ic::UI::Global::get().focusedTextField->set_focused(false);
            SDL_StopTextInput();
        }
    });

    this->mouse->add_mouse_down_action([this]() {
        this->mainTable->mouse_down();
    });

    ic::InputHandler::add_input(this->mouse, "ui mouse");
}


void ic::UI::Core::handle_event(ic::Event event, float dt) {
    if (event.type == SDL_TEXTINPUT) {
        if (ic::UI::Global::get().focusedTextField != nullptr) {
            ic::UI::Global::get().focusedTextField->input_text(*event.text.text);
        }
    }

    if (event.type == SDL_KEYDOWN) {
        if (ic::UI::Global::get().focusedTextField != nullptr) {
           ic::UI::Global::get().focusedTextField->input_key(&event.key);
        }
    }
}


void ic::UI::Core::update(float dt) {
    this->mainTable->update(dt);
    this->mainTable->recalculate_size();
}


void ic::UI::Core::render(float dt) {
    this->mainTable->draw();

    glCullFace(GL_BACK);
    glDepthMask(GL_FALSE);

    // Textures
    this->uiShader.use();
    this->uiCamera.use(this->uiShader);

    ic::UI::Global::get().atlas.use();
    ic::UI::Global::get().fillBatch.render();


    // Text
    this->uiTextShader.use();
    this->uiCamera.use(this->uiTextShader);
    
    // Currently uses the default atlas
    ic::UI::Global::get().defaultAtlas.use();
    ic::UI::Global::get().fillTextBatch.render();

    glDepthMask(GL_TRUE);
    glCullFace(GL_FRONT);
}



void ic::UI::Core::update_and_render(float dt) {
    this->update(dt);
    this->render(dt);
}


void ic::UI::Core::dispose() {
    this->uiShader.clear();
    this->uiTextShader.clear();

    ic::UI::Global::get().fillBatch.dispose();
    ic::UI::Global::get().fillTextBatch.dispose();
}