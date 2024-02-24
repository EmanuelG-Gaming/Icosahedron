#ifndef IC_UI_GLOBAL_H
#define IC_UI_GLOBAL_H

#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/graphics/gl/Batch.h>
#include <Icosahedron/Renderer.h>

#include <Icosahedron/graphics/TextAtlas.h>
#include <Icosahedron/graphics/gl/TextureAtlas.h>
#include <Icosahedron/graphics/FreeType.h>

#include <Icosahedron/input/MouseController.h>


namespace ic { namespace UI {
    namespace Global {
        ic::Batch fillBatch, fillTextBatch;
        ic::TextAtlas defaultAtlas;
        ic::TextureAtlas atlas;
        ic::Vec2f mouseCursorPosition;

        void load();
    };
}}

#endif