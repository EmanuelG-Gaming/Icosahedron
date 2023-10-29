#ifndef IC_UI_LABEL_H
#define IC_UI_LABEL_H

#include <string>

#include <Icosahedron/graphics/TextAtlas.h>
#include <Icosahedron/graphics/FreeType.h>

#include <UI/Element.h>
#include <UI/style/Style.h>
#include <UI/style/Drawable.h>
#include <UI/Global.h>


namespace ic { namespace UI {
    class LabelStyle : public Style {
        public:
            ic::TextAtlas *font;
            ic::Color fontColor;
            ic::UI::Drawable *background;

            LabelStyle() {
                this->font = ic::UI::Global::get().defaultAtlas;
                this->fontColor = { 255, 255, 255 };
                this->background = nullptr;
            }

            LabelStyle(const std::string &fontName, ic::UI::Drawable *background = nullptr) {
                this->font = ic::FreeType::get().find_atlas(fontName);
                this->fontColor = { 255, 255, 255 };
                this->background = background;
            }

            LabelStyle(const std::string &fontName, const ic::Color &color, ic::UI::Drawable *background = nullptr) {
                this->font = ic::FreeType::get().find_atlas(fontName);
                this->fontColor = color;
                this->background = background;
            }
    };


    class Label : public Element {
        public:
            std::string text;
            LabelStyle style;

            Label(const std::string &text);


            void draw() override;

            ic::UI::Label *set_style(const ic::UI::LabelStyle &style);
            ic::UI::Label *set_font_color(const ic::Color &color);
            ic::UI::Label *set_background(ic::UI::Drawable *background);
    };
}}

#endif