#ifndef IC_UI_LABEL_H
#define IC_UI_LABEL_H

#include <string>

#include <Icosahedron/graphics/TextAtlas.h>

#include <UI/Element.h>
#include <UI/style/Drawable.h>
#include <UI/Global.h>


namespace ic { namespace UI {
    class LabelStyle {
        public:
            ic::TextAtlas font;
            ic::UI::Drawable *background;
            ic::Color fontColor;
            
            LabelStyle() {
                this->fontColor = ic::Colors::white;
                this->background = nullptr;
                this->font = ic::UI::Global::get().defaultAtlas;
            }

            LabelStyle(ic::TextAtlas &font, ic::UI::Drawable *background = nullptr) {
                this->font = font;
                this->fontColor = ic::Colors::white;
                this->background = background;
            }
    };


    class Label : public Element {
        public:
            LabelStyle style;

            Label(const std::string &text);


            void draw() override;

            ic::UI::Label *set_style(const ic::UI::LabelStyle &style);
            ic::UI::Label *set_font_color(const ic::Color &color);
            ic::UI::Label *set_background(ic::UI::Drawable *background);

            ic::UI::Label *set_text(const std::string &text);

            std::string &get_text();
        
        protected:
            std::string text;
    };
}}

#endif