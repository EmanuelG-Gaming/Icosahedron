#include <UI/TextField.h>

using namespace ic::UI;

TextField::TextField() {
    this->text = "";
    this->filter = TextFieldFilters::characters;
    this->width = this->height = 0.3f;
    this->positiveInput = false;
}


TextField::TextField(std::string text, TextFieldFilters filter, float width, float height, bool positiveInput) {
    this->text = text;
    this->filter = filter;
    this->width = width;
    this->height = height;
    this->positiveInput = positiveInput;
}


void TextField::draw() {
    if (this->style.background != nullptr) {
        this->style.background->draw(this->translation.x(), this->translation.y(), this->width, this->height);
    }

    if (this->style.focused != nullptr && this->focused) {
        this->style.focused->draw(this->translation.x(), this->translation.y() - 0.005f, this->width - 0.01f, 0.005f);
    }

    ic::Renderer::draw_string(ic::UI::Global::get().fillTextBatch, this->style.font, this->text, this->translation.x() - this->width + 0.005f, this->translation.y() - this->height, 1.0f, 1.0f, this->style.fontColor);
}

void TextField::mouse_down_callback() {
    bool contains = this->contains(ic::UI::Global::get().mouseCursorPosition);
    if (contains) {
        SDL_StartTextInput();
        set_focused(true);
    }
}



void TextField::input_text(char input) {
    if (filter == TextFieldFilters::characters) {
        this->text += input;
        std::cout << this->text << "\n";
    } else if (filter == TextFieldFilters::integers) {
        bool sign = !this->positiveInput && input == '-' && this->text.find("-") == std::string::npos;
        
        if (isdigit(input) || sign) {
            this->text += input;
        }
    } else if (filter == TextFieldFilters::floats) {
        bool isRealNumber = input == '.' && this->text.find(".") == std::string::npos;
        bool sign = !this->positiveInput && input == '-' && this->text.find("-") == std::string::npos;
        
        if (isdigit(input) || isRealNumber || sign) {
            this->text += input;
        }
    }
}

void TextField::input_key(SDL_KeyboardEvent *event) {
    if (event->keysym.scancode == SDL_SCANCODE_BACKSPACE && text.size()) {
        text.pop_back();
    }
}


TextField *TextField::set_focused(bool to) {
    this->focused = to;
    if (to) {
        Global::get().focusedTextField = this;
    } else if (Global::get().focusedTextField == this) {
        Global::get().focusedTextField = nullptr;
    }
              
    return this;
}

bool TextField::is_focused() {
    return this->focused;
}


TextField *TextField::set_text(const std::string &to) {
    this->text = to;

    return this;
}

std::string &TextField::get_text() {
    return this->text;
}

ic::UI::TextField *ic::UI::TextField::set_style(const ic::UI::TextFieldStyle &style) {
    this->style = style;
    
    return this;
}


bool ic::UI::TextField::contains(ic::Vec2f &point) {
    return (point.x() >= this->translation.x() - this->width && point.x() <= this->translation.x() + this->width) &&
           (point.y() >= this->translation.y() - this->height && point.y() <= this->translation.y() + this->height);
}