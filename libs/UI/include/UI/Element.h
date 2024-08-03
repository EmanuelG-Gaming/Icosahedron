#ifndef IC_UI_ELEMENT_H
#define IC_UI_ELEMENT_H

#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/input/Input.h>

namespace ic { namespace UI {
    class Element {
        public:
            /** @brief The total displacement of this element over the UI scene. */
            ic::Vec2f translation;

            /** @brief Position that is relative to the table. */
            ic::Vec2f relativePosition;

            /** @brief Size of the bounding box. */
            float width = 0.0f, height = 0.0f;
            
            Element();


            virtual void draw() {}

            virtual void mouse_moved_callback() {}
            virtual void mouse_up_callback() {}
            virtual void mouse_down_callback() {}

            virtual void event_callback(const ic::Event &event) {}
            

            virtual ic::UI::Element *set_position(float x, float y, bool insideTable = false);
            virtual ic::UI::Element *set_position(ic::Vec2f to, bool insideTable = false);
            
            virtual ic::UI::Element *set_size(float w, float h);
            virtual ic::UI::Element *set_size(ic::Vec2f to);
    };
}}

#endif