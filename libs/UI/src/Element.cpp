#include <UI/Element.h>

using namespace ic::UI;


Element::Element() {
    this->width = this->height = 0.0f;
}


ic::UI::Element *ic::UI::Element::set_position(float x, float y) {
    this->relativePosition.x() = this->translation.x() = x;
    this->relativePosition.y() = this->translation.y() = y;

    return this;
}

ic::UI::Element *ic::UI::Element::set_position(ic::Vec2f to) {
    return this->set_position(to.x(), to.y());
}


ic::UI::Element *ic::UI::Element::set_size(float w, float h) {
    this->width = w;
    this->height = h;

    return this;
}

ic::UI::Element *ic::UI::Element::set_size(ic::Vec2f to) {
    return this->set_size(to.x(), to.y());
}