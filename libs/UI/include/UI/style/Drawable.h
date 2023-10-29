#ifndef IC_UI_DRAWABLE_H
#define IC_UI_DRAWABLE_H


namespace ic { namespace UI {
    class Drawable {
        public:
            virtual void draw(float x, float y, float width, float height) {}
    };
}}
#endif