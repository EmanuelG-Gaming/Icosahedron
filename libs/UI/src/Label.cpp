#include <UI/Label.h>
#include <UI/Global.h>

using namespace ic::UI;


Label::Label(const std::string &text) {
    this->set_text(text);
}


void ic::UI::Label::draw() {
    if (this->style.background != nullptr) {
        this->style.background->draw(this->translation.x(), this->translation.y(), this->width, this->height);
    }

    ic::Renderer::draw_string(ic::UI::Global::get().fillTextBatch, this->style.font, this->text, this->translation.x() - this->width, this->translation.y() - this->height / 2.0f, 0.1f, this->style.fontColor);
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



ic::UI::Label *ic::UI::Label::set_text(const std::string &text) {
    this->text = text;
    
    float w = ic::Renderer::calculate_width(this->style.font, this->text, 0.1f);

    this->width = w / 2.0f;
    this->height = 0.1f / 2.0f;

    return this;
}

std::string &ic::UI::Label::get_text() {
    return this->text;
}