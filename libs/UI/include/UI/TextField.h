#ifndef IC_UI_TEXT_FIELD_H
#define IC_UI_TEXT_FIELD_H

#include <functional>

#include <UI/Element.h>
#include <UI/style/TextureDrawable.h>


namespace ic { namespace UI {
    enum class TextFieldFilters { 
        integers,
        floats,
        characters,
    };

    class TextFieldStyle {
        public:
            ic::TextAtlas font;
            ic::Color fontColor;
            Drawable *background, *focused;
    
            TextFieldStyle() {
                this->background = this->focused = nullptr;
                this->fontColor = ic::Colors::white;
                this->font = ic::UI::Global::get().defaultAtlas;
            }

            TextFieldStyle(const ic::TextAtlas &font) {
                this->background = this->focused = nullptr;
                this->fontColor = ic::Colors::white;
                this->font = font;
            }

            TextFieldStyle(Drawable *back, Drawable *focus, const ic::Color &fontCol, const ic::TextAtlas &font) {
                this->background = back;
                this->focused = focus;
                this->fontColor = fontCol;
                
                this->font = font;
            }
    };

    class TextField : public Element {
        public:
            TextFieldStyle style;
            std::function<void()> submitCallback;
            std::function<void(char)> letterInputCallback;


            TextField();
            TextField(std::string text, TextFieldFilters filter, float width, float height, bool positiveInput = false);


            void draw() override;

            void mouse_down_callback() override;
            void mouse_moved_callback() override;
        
            void input_text(char input);
            void input_key(SDL_KeyboardEvent *event);


            TextField *set_focused(bool to);
            bool is_focused();
            
            TextField *set_text(const std::string &to);
            std::string &get_text();

            ic::UI::TextField *set_style(const ic::UI::TextFieldStyle &style);

        protected:
            bool contains(ic::Vec2f &point);

        protected:
            std::string text;
            TextFieldFilters filter;

            bool focused;
            bool positiveInput;
            float width, height;
    };
}}

#endif