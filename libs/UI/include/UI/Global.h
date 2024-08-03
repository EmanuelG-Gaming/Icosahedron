#ifndef IC_UI_GLOBAL_H
#define IC_UI_GLOBAL_H

#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/graphics/gl/Batch.h>
#include <Icosahedron/Renderer.h>

#include <Icosahedron/graphics/TextAtlas.h>
#include <Icosahedron/graphics/gl/TextureAtlas.h>
#include <Icosahedron/assets/loaders/FontLoader.h>

#include <Icosahedron/input/MouseController.h>


namespace ic { namespace UI {
    class TextField;

    class Global {
        public:
            ic::Batch fillBatch, fillTextBatch;
            ic::TextureAtlas atlas;
            ic::TextAtlas defaultAtlas;

            ic::Vec2f mouseCursorPosition;
            
            ic::UI::TextField *focusedTextField;


            static Global& get() {
                static Global ins;
                return ins;
            }

            void load();

        private:
            Global() {}
            ~Global() {}

        public:
            Global(Global const&) = delete;
            void operator = (Global const&) = delete;

    };
}}

#endif