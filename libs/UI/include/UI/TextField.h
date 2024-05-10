#ifndef IC_UI_TEXT_FIELD
#define IC_UI_TEXT_FIELD


#include <UI/Element.h>


namespace ic { namespace UI {
    class TextField : public Element {
        public:
            /** @brief The total displacement of this element over the UI scene. */
            ic::Vec2f translation;

            /** @brief Position that is relative to the table. */
            ic::Vec2f relativePosition;

            /** @brief Size of the bounding box. */
            float width, height;
            
            TextField();


            virtual void draw() {}

            virtual void mouse_moved_callback() {}
            virtual void mouse_up_callback() {}
            virtual void mouse_down_callback() {}

            

            virtual ic::UI::TextField *set_position(float x, float y, bool insideTable = false);
            virtual ic::UI::TextField *set_position(ic::Vec2f to, bool insideTable = false);
            
            virtual ic::UI::TextField *set_size(float w, float h);
            virtual ic::UI::TextField *set_size(ic::Vec2f to);
    };
}}

#endif