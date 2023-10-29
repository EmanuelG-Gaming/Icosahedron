#ifndef IC_UI_ELEMENT_H
#define IC_UI_ELEMENT_H

#include <Icosahedron/math/geom/Vectors.h>

namespace ic { namespace UI {
    class Element {
        public:
            ic::Vec2f translation;
            float width, height;
            
            Element();


            virtual void draw() {}


            ic::UI::Element *set_position(float x, float y);
            ic::UI::Element *set_position(ic::Vec2f to);
    };
}}

#endif