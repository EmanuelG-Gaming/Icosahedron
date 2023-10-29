#ifndef IC_UI_GLOBAL_H
#define IC_UI_GLOBAL_H

#include <Icosahedron/graphics/gl/Batch.h>
#include <Icosahedron/Renderer.h>

#include <Icosahedron/graphics/TextAtlas.h>
#include <Icosahedron/graphics/FreeType.h>


namespace ic { namespace UI {
    class Global {
        public:
            ic::Renderer renderer;
            ic::Batch *fillBatch, *fillTextBatch;
            ic::TextAtlas *defaultAtlas;
        
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