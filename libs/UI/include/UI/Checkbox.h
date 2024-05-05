#ifndef IC_UI_CHECKBOX_H
#define IC_UI_CHECKBOX_H

#include <UI/Button.h>

namespace ic { namespace UI {
    class CheckboxStyle {
        public:
            Drawable *on, *off;
    
            CheckboxStyle() {
                on = off = nullptr;
            }
    
            CheckboxStyle(Drawable *on, Drawable *off) {
                this->on = on;
                this->off = off;
            }
    };

    class Checkbox : public Button {
        public:
            CheckboxStyle checkboxStyle;

            Checkbox();
            Checkbox(const std::function<bool()> &callback);

            virtual void mouse_up_callback() override;

            void draw() override;

            ic::UI::Checkbox *set_style(const ic::UI::CheckboxStyle &style);

        protected:
            bool on;
            std::function<bool()> clicked;

    };
}}

#endif