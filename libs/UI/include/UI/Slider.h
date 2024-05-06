#ifndef IC_UI_SLIDER_H
#define IC_UI_SLIDER_H

#include <functional>

#include <Icosahedron/math/geom/Vectors.h>

#include <UI/style/Drawable.h>
#include <UI/Element.h>
#include <UI/Global.h>


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



    class Slider : public Element {
        public:
            SliderStyle style;
            
            Slider();
            Slider(float t, float lineWidth, bool vertical, const std::function<float()> &knobMoved);


            void draw() override;

            virtual void mouse_moved_callback() override;
            virtual void mouse_up_callback() override;
            virtual void mouse_down_callback() override;
            

            ic::UI::Slider *set_style(const ic::UI::SliderStyle &style);

        protected:
            bool knob_contains(ic::Vec2f &point);
            ic::Vec2 calculate_rotated_sizes(float baseWidth, float baseHeight);

        protected:
            bool vertical;
            bool draggingKnob;
            float lineWidth, lineThickness;
            float knobWidth, knobHeight;

            ic::Vec2 previousKnobPosition, currentKnobPosition;

            std::function<void(float)> knobMovedCallback;
    };
}}

#endif