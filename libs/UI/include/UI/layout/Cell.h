#ifndef IC_UI_CELL_H
#define IC_UI_CELL_H

#include <UI/Element.h>


namespace ic { namespace UI {
    class Table;

    /** @brief Leaf of a table tree. */
    class Cell {
        public:
            Element *element;
            ic::Vec2f elementPosition;


            Cell();

            void set_layout(Table *table);

            bool has_element();

        private: 
            Table *table;
    };
}}

#endif