#ifndef IC_UI_BUTTON_H
#define IC_UI_BUTTON_H

#include <functional>

#include <UI/style/Drawable.h>

#include <UI/layout/Table.h>


namespace ic { namespace UI {
    class ButtonStyle {
        public:
            Drawable *base, *checked, *pressed, *disabled;
    
            ButtonStyle() {
                base = checked = pressed = disabled = nullptr;
            }
    
            ButtonStyle(Drawable *base, Drawable *checked, Drawable *pressed) {
                this->base = base;
                this->checked = checked;
                this->pressed = pressed;
            }
    };



    class Button : public Table {
        public:
            ButtonStyle style;
            std::function<void()> clickListener;

            Button();


            void mouse_moved_callback() override;
            void mouse_up_callback() override;
            void mouse_down_callback() override;
            
            void draw() override;

            ic::UI::Button *set_style(const ic::UI::ButtonStyle &style);

        protected:
            bool contains(ic::Vec2f &point);

        protected:
            bool isPressed, isChecked;
    };
}}

#endif