#ifndef IC_UI_TABLE_H
#define IC_UI_TABLE_H

#include <vector>
#include <functional>
#include <string>

#include <UI/layout/Cell.h>
#include <UI/style/Drawable.h>
#include <UI/Label.h>


namespace ic { namespace UI {
    class Button;

    class Table {
        public:
            ic::Vec2f position;

            Table();
            Table(Drawable *background);

            Table(const std::function<void(Table*)> &consumer);
            Table(Drawable *background, const std::function<void(Table*)> &consumer);

            virtual void mouse_moved_callback() {}
            virtual void mouse_up_callback() {}
            virtual void mouse_down_callback() {}

            virtual void draw();
            
            void mouse_moved();
            void mouse_up();
            void mouse_down();

            /* UI elements */

            Cell *add(Element *element);
            Table *add(Table *table);

            Table *table(const std::function<void(Table*)> &consumer);
            Label *label(const std::string &text);
            Button *button();
            Button *button(const std::function<void()> &clicked);


            Table *set_background(Drawable *drawable);
            bool has_background();
        
        protected:
            void draw_elements();

        protected:
            Drawable *background;

            std::vector<Cell*> cells;
            std::vector<Table*> tables;
    };
}}

#endif