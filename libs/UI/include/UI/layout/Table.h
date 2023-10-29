#ifndef IC_UI_TABLE_H
#define IC_UI_TABLE_H

#include <vector>

#include <UI/layout/Cell.h>
#include <UI/style/Drawable.h>


namespace ic { namespace UI {
    class Table {
        public:
            ic::Vec2f position;

            Table();

            void draw();


            /* UI elements */
            Cell *add(Element *element);
            Table *add(Table *table);



            Table *set_background(Drawable *drawable);
            bool has_background();
            
        protected:
            Drawable *background;

            std::vector<Cell*> cells;
            std::vector<Table*> tables;
    };
}}

#endif