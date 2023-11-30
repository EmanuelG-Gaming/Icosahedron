#ifndef IC_UI_IMAGE_ELEMENT_H
#define IC_UI_IMAGE_ELEMENT_H

#include <string>

#include <UI/Element.h>

#include <UI/style/Drawable.h>
#include <UI/style/TextureDrawable.h>

#include <UI/Global.h>


namespace ic { namespace UI {
    class ImageStyle {
        public:
            ic::UI::Drawable *front;

            ImageStyle() {
                this->front = new ic::UI::TextureDrawable();
            }

            ImageStyle(const AtlasEntry &entry) {
                this->front = new ic::UI::TextureDrawable(entry);
            }

            ImageStyle(const std::string &entryName) {
                this->front = new ic::UI::TextureDrawable(entryName);
            }
    };


    class ImageElement : public Element {
        public:
            ImageStyle style;

            ImageElement(const std::string &atlasEntryName, float width, float height);


            void draw() override;

            ic::UI::ImageElement *set_style(const ic::UI::ImageStyle &style);
    };
}}

#endif