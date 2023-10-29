#ifndef IC_UI_LABEL_H
#define IC_UI_LABEL_H

#include <string>

#include <UI/Element.h>


namespace ic { namespace UI {
    class Label : public Element {
        public:
            std::string text;

            Label(std::string text);


            void draw() override;
    };
}}

#endif