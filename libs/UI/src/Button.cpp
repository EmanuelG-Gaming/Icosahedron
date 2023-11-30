#include <UI/Button.h>

using namespace ic::UI;

Button::Button() {
    this->clickListener = nullptr;
    this->isPressed = false;
    this->isChecked = false;
}


void ic::UI::Button::mouse_moved_callback() {
    bool before = this->isChecked;
    this->isChecked = this->contains(ic::UI::Global::get().mouseCursorPosition);

    if (this->isChecked != before) {
        if (!this->isChecked) {
            // When the cursor leaves the area
            //printf("mouse leaves\n");
        } else {
            // When the cursor enters the area
            //printf("mouse enters\n");
        }
    }
}

void ic::UI::Button::mouse_up_callback() {
    if (this->isPressed && this->clickListener != nullptr) {
        this->clickListener();
    }
    this->isPressed = false;
}

void ic::UI::Button::mouse_down_callback() {
    if (this->isChecked) {
        this->isPressed = true;
    }
}

void ic::UI::Button::draw() {
    Drawable *top = this->style.base;
    if (this->clickListener != nullptr) {
        if (this->isChecked) {
            top = this->style.checked;
        }
        if (this->isPressed) {
            top = this->style.pressed;
        }
    }


    if (this->has_background()) {
        this->background->draw(this->position.x(), this->position.y(), 0.1f, 0.1f);
    }

    if (top != nullptr) {
        top->draw(this->position.x(), this->position.y(), 0.1f, 0.1f);
    }


    this->draw_elements();
}


ic::UI::Button *ic::UI::Button::set_style(const ic::UI::ButtonStyle &style) {
    this->style = style;

    return this;
}
            
bool ic::UI::Button::contains(ic::Vec2f &point) {
    return (point.x() >= position.x() - 0.1f && point.x() <= position.x() + 0.1f) &&
           (point.y() >= position.y() - 0.1f && point.y() <= position.y() + 0.1f);
}