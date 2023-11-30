#include <UI/ImageElement.h>
#include <UI/Global.h>

using namespace ic::UI;


ImageElement::ImageElement(const std::string &atlasEntryName, float width, float height) {
    this->style = ic::UI::ImageStyle(atlasEntryName);
    this->width = width;
    this->height = height;
}


void ic::UI::ImageElement::draw() {
    if (this->style.front != nullptr) {
        this->style.front->draw(this->translation.x(), this->translation.y(), this->width, this->height);
    }
}



ic::UI::ImageElement *ic::UI::ImageElement::set_style(const ic::UI::ImageStyle &style) {
    this->style = style;

    return this;
}