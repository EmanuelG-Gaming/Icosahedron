#include <UI/Checkbox.h>

using namespace ic::UI;

Checkbox::Checkbox() : Button() {
    this->on = false;
    this->toChange = NULL;
}

Checkbox::Checkbox(bool *toChange) : Checkbox() {
    this->toChange = toChange;
}


void ic::UI::Checkbox::draw() {
    Drawable *top = this->style.base;
    Drawable *checks = this->checkboxStyle.off;

    if (this->clickListener != nullptr) {
        if (this->isChecked) {
            top = this->style.checked;
        }
        if (this->isPressed) {
            top = this->style.pressed;
        }

        if (this->on) {
            checks = this->checkboxStyle.on;
        }
    }


    if (this->has_background()) {
        this->background->draw(this->position.x(), this->position.y(), this->width, this->height);
    }

    if (top != nullptr) {
        top->draw(this->position.x(), this->position.y(), this->width, this->height);
    }

    if (checks != nullptr) {
        checks->draw(this->position.x(), this->position.y(), this->width, this->height);
    }


    this->draw_elements();
}


ic::UI::Checkbox *ic::UI::Checkbox::set_style(const ic::UI::CheckboxStyle &style) {
    this->checkboxStyle = style;

    return this;
}