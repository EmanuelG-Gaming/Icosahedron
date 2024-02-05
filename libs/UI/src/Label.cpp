#include <UI/Label.h>
#include <UI/Global.h>

using namespace ic::UI;


Label::Label(const std::string &text) {
    this->set_text(text);
}


void ic::UI::Label::draw() {
    if (this->style.background != nullptr) {
        float halfWidth = this->width / 2.0f;
        float halfHeight = this->height / 2.0f;

        this->style.background->draw(this->translation.x() + halfWidth, this->translation.y() + halfHeight, halfWidth, halfHeight);
    }

    //ic::UI::Global::get().renderer.draw_string(ic::UI::Global::get().fillTextBatch, this->style.font, this->text, this->translation.x(), this->translation.y(), 1.0f, 1.0f, this->style.fontColor);
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
    //this->text = text;
//
    //ic::Vec2f size = this->compute_size(this->style.font);
    //this->width = size.x();
    //this->height = size.y();
//
    //return this;
}

std::string &ic::UI::Label::get_text() {
    return this->text;
}


/*
ic::Vec2f ic::UI::Label::compute_size(ic::TextAtlas &font) {
    float sclX = 1.0f * 0.002f;
    float sclY = 1.0f * 0.002f;
        
    float w = 0.0f;
    float h = 0.0f;
    std::string::const_iterator iterator;
    for (iterator = text.begin(); iterator != text.end(); iterator++) {
        CharacterInfo ch = font.get_characters().at(*iterator);
                
        w += ch.advX;
                
        if (ch.bitmapHeight > h) {
            h = ch.bitmapHeight;
        }
    }
    w *= sclX;
    h *= sclY;

    ic::Vec2f result = { w, h };

    return result;
}
*/