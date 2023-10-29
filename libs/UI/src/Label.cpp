#include <UI/Label.h>
#include <UI/Global.h>

using namespace ic::UI;


Label::Label(const std::string &text) {
    this->text = text;
}


void ic::UI::Label::draw() {
    if (this->style.background != nullptr) {
        this->style.background->draw(this->translation.x(), this->translation.y(), 0.1f, 0.1f);
    }

    ic::UI::Global::get().renderer.draw_string(ic::UI::Global::get().fillTextBatch, this->style.font, this->text, this->translation.x(), this->translation.y(), 1.0f, 1.0f, this->style.fontColor);
}



ic::UI::Label *ic::UI::Label::set_style(const ic::UI::LabelStyle &style) {
    this->style = style;

    return this;
}

ic::UI::Label *ic::UI::Label::set_font_color(const ic::Color &color) {
    this->style.fontColor = color;

    return this;
}

ic::UI::Label *ic::UI::Label::set_background(ic::UI::Drawable *background) {
    this->style.background = background;

    return this;
}