#ifndef IC_UI_CORE_H
#define IC_UI_CORE_H

#include <Icosahedron/graphics/gl/Shader.h>

#include <Icosahedron/scene/2d/Camera2D.h>

#include <UI/layout/Table.h>
#include <UI/Global.h>


namespace ic { namespace UI {
    class Core {
        public:
            Table *mainTable;
            ic::TextureAtlas *atlas;

            static Core& get() {
                static Core ins;
                return ins;
            }

            void load();

            void handle_event(ic::Event event, float dt);

            void update(float dt);
            void render(float dt);

            void update_and_render(float dt);
            
            void dispose();
        
        private:
            Core() {}
            ~Core() {}

        public:
            Core(Core const&) = delete;
            void operator = (Core const&) = delete;

        private:
            ic::Camera2D uiCamera;
            ic::Shader uiTextShader, uiShader;

            ic::MouseController *mouse;
    };
}}

#endif