#include <UI/Element.h>

using namespace ic::UI;


Element::Element() {
    this->width = this->height = 0.0f;
}


ic::UI::Element *ic::UI::Element::set_position(float x, float y) {
    this->translation.x() = x;
    this->translation.y() = y;

    return this;
}

ic::UI::Element *ic::UI::Element::set_position(ic::Vec2f to) {
    return this->set_position(to.x(), to.y());
}