#ifndef IC_UI_ELEMENT_H
#define IC_UI_ELEMENT_H

#include <Icosahedron/math/geom/Vectors.h>

namespace ic { namespace UI {
    class Element {
        public:
            /** @brief The total displacement of this element over the UI scene. */
            ic::Vec2f translation;

            /** @brief Position that is relative to the table. */
            ic::Vec2f relativePosition;

            /** @brief Size of the bounding box. */
            float width, height;
            
            Element();


            virtual void draw() {}


            ic::UI::Element *set_position(float x, float y);
            ic::UI::Element *set_position(ic::Vec2f to);

            ic::UI::Element *set_size(float w, float h);
            ic::UI::Element *set_size(ic::Vec2f to);
    };
}}

#endif