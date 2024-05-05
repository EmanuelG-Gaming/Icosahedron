#ifndef IC_UI_SLIDER_H
#define IC_UI_SLIDER_H

#include <functional>

#include <Icosahedron/math/geom/Vectors.h>

#include <UI/style/Drawable.h>
#include <UI/layout/Table.h>



namespace ic { namespace UI {
    class SliderStyle {
        public:
            Drawable *line, *knob;
    
            SliderStyle() {
                line = knob = nullptr;
            }
    
            SliderStyle(Drawable *line, Drawable *knob) {
                this->line = line;
                this->knob = knob;
            }
    };



    class Slider : public Table {
        public:
            SliderStyle style;
            std::function<void()> clickListener;

            Slider();
            Slider(float t, float lineWidth, bool vertical, const std::function<float()> &knobMoved);



            virtual void mouse_moved_callback() override;
            virtual void mouse_up_callback() override;
            virtual void mouse_down_callback() override;
            
            void draw() override;


            ic::UI::Slider *set_style(const ic::UI::SliderStyle &style);

        protected:
            bool contains(ic::Vec2f &point);

        protected:
            bool vertical;
            float lineWidth, lineThickness;
            float knobWidth, knobHeight;

            ic::Vec2 previousKnobPosition, currentKnobPosition;

            std::function<float()> knobMovedCallback;
    };
}}

#endif