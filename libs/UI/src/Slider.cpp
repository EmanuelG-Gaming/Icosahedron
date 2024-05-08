#include <UI/Slider.h>

using namespace ic::UI;

Slider::Slider() {
    this->previousKnobPosition = this->currentKnobPosition = this->translation;
    this->knobMovedCallback = nullptr;
    
    this->vertical = false;
    this->draggingKnob = false;

    this->lineWidth = 0.3f;
    this->lineThickness = 0.025f;

    this->knobWidth = 0.04f;
    this->knobHeight = 0.06f;
}


Slider::Slider(float t, float lineWidth, bool vertical, const std::function<void(float)> &knobMoved) : Slider() {
    this->vertical = vertical;
    this->lineWidth = lineWidth;
    this->knobMovedCallback = knobMoved;
}

Slider::Slider(const std::function<void(float)> &knobMoved) : Slider() {
    this->knobMovedCallback = knobMoved;
}


void Slider::draw() {
    ic::Vec2 knobSizes = this->calculate_rotated_sizes(this->knobWidth, this->knobHeight);
    ic::Vec2 lineSizes = this->calculate_rotated_sizes(this->lineWidth, this->lineThickness);

    if (this->style.line != nullptr) {
        this->style.line->draw(this->translation.x(), this->translation.y(), lineSizes.x(), lineSizes.y());
    }

    if (this->style.knob != nullptr) {
        this->style.knob->draw(this->currentKnobPosition.x(), this->currentKnobPosition.y(), knobSizes.x(), knobSizes.y());
    }
}


void Slider::mouse_moved_callback() {
    ic::Vec2 cursorPos = Global::get().mouseCursorPosition;

    if (!this->draggingKnob) {
        return;
    } 

    if (vertical) {
        this->currentKnobPosition.y() = ic::Mathf::clamp(cursorPos.y(), this->translation.y() - this->lineWidth, this->translation.y() + this->lineWidth);
    } else {
        this->currentKnobPosition.x() = ic::Mathf::clamp(cursorPos.x(), this->translation.x() - this->lineWidth, this->translation.x() + this->lineWidth);
    }
    
    float lower = (vertical ? this->translation.y() : this->translation.x());
    lower -= this->lineWidth;

    float t = ((vertical ? this->previousKnobPosition.y() : this->currentKnobPosition.x()) - lower) / (this->lineWidth * 2.0f);
    if (this->knobMovedCallback != nullptr) {
        this->knobMovedCallback(t);
    }

    this->previousKnobPosition = this->currentKnobPosition;
}

void Slider::mouse_up_callback() {
    this->draggingKnob = false;
}

void Slider::mouse_down_callback() {
    ic::Vec2 cursorPos = Global::get().mouseCursorPosition;

    if (!this->knob_contains(cursorPos)) {
        return;
    }

    this->draggingKnob = true;

    if (vertical) {
        this->previousKnobPosition.y() = this->currentKnobPosition.y() = ic::Mathf::clamp(cursorPos.y(), this->translation.y() - this->lineWidth, this->translation.y() + this->lineWidth);
    } else {
        this->previousKnobPosition.x() = this->currentKnobPosition.x() = ic::Mathf::clamp(cursorPos.x(), this->translation.x() - this->lineWidth, this->translation.x() + this->lineWidth);
    }
}


bool Slider::knob_contains(ic::Vec2 &point) {
    ic::Vec2 sizes = this->calculate_rotated_sizes(this->knobWidth, this->knobHeight);

    return (point.x() >= this->currentKnobPosition.x() - sizes.x() && point.x() <= this->currentKnobPosition.x() + sizes.x()) &&
           (point.y() >= this->currentKnobPosition.y() - sizes.y() && point.y() <= this->currentKnobPosition.y() + sizes.y());
}


ic::Vec2 Slider::calculate_rotated_sizes(float baseWidth, float baseHeight) {
    float w = baseWidth, h = baseHeight;

    // When the slider is vertical, the knob is rotated 90 degrees relative to horizontal
    // so swap the sizes accordingly
    if (vertical) {
        w = baseHeight;
        h = baseWidth;
    }

    return ic::Vec2(w, h);
}

ic::UI::Slider *ic::UI::Slider::set_style(const ic::UI::SliderStyle &style) {
    this->style = style;

    return this;
}

ic::UI::Slider *ic::UI::Slider::set_position(float x, float y, bool insideTable) {
    if (insideTable) {
        this->translation.x() = this->currentKnobPosition.x() = this->previousKnobPosition.x() = this->relativePosition.x() + x;
        this->translation.y() = this->currentKnobPosition.y() = this->previousKnobPosition.y() = this->relativePosition.y() + y;
    } else {
        this->translation.x() = this->currentKnobPosition.x() = this->previousKnobPosition.x() = this->relativePosition.x() = x;
        this->translation.y() = this->currentKnobPosition.y() = this->previousKnobPosition.y() = this->relativePosition.y() = y;
    }

    return this;
}

ic::UI::Slider *ic::UI::Slider::set_position(ic::Vec2f to, bool insideTable) {
    return this->set_position(to.x(), to.y(), insideTable);
}