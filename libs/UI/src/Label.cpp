#include <UI/Label.h>
#include <UI/Global.h>

using namespace ic::UI;


Label::Label(std::string text) {
    this->text = text;
}


void ic::UI::Label::draw() {
    ic::UI::Global::get().renderer.draw_string(ic::UI::Global::get().fillBatch, ic::UI::Global::get().defaultAtlas, this->text, this->translation.x(), this->translation.y());
}