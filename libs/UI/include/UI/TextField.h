#ifndef IC_UI_TEXT_FIELD
#define IC_UI_TEXT_FIELD


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
                background = focused = nullptr;
                fontColor = ic::Colors::white;
                font = ic::UI::Global::get().defaultAtlas;
            }

            TextFieldStyle(const std::string &fontName) {
                background = focused = nullptr;
                fontColor = ic::Colors::white;
                font = ic::FreeType::find_atlas(fontName);
            }

            TextFieldStyle(Drawable *back, Drawable *focus, const ic::Color &fontCol = ic::Colors::white, const std::string &fontName = "") {
                background = back;
                focused = focus;
                fontColor = fontCol;
                
                if (!fontName.empty()) {
                    font = ic::FreeType::find_atlas(fontName);
                } else {
                    font = ic::UI::Global::get().defaultAtlas;
                }
            }
    };

    class TextField : public Element {
        public:
            TextFieldStyle style;

            TextField();
            TextField(std::string text, TextFieldFilters filter, float width, float height, bool positiveInput = false);


            void draw() override;

            void mouse_down_callback() override;

        
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