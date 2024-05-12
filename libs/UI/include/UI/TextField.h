#ifndef IC_UI_TEXT_FIELD
#define IC_UI_TEXT_FIELD


#include <UI/Element.h>


namespace ic { namespace UI {
    enum class TextFieldFilters { 
        integers,
        floats,
        characters,
    };

    class TextField : public Element {
        public:
            TextField();
            TextField(std::string label, std::string text, TextFieldFilters filter, bool positiveInput);
            TextField(std::string label, TextFieldFilters filter, bool positiveInput);
            TextField(std::string label, TextFieldFilters filter);


            virtual void draw() {}

            virtual void mouse_moved_callback() {}
            virtual void mouse_up_callback() {}
            virtual void mouse_down_callback() {}
         
         
        
            void input_text(char input);
            void input_key(SDL_KeyboardEvent *event);


            TextField *set_focused(bool to);
            bool is_focused();
            
            TextField *set_labelPaddingX(float x);
        
            TextField *set_text(const std::string &to);
            std::string &get_text();

        private:
            std::string text;
            TextFieldFilters filter;
            
            bool focused;
            bool positiveInput;
            float labelPadX;
    };
}}

#endif