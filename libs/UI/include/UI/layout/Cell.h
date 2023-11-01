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

            virtual void mouse_moved_callback() {}
            virtual void mouse_up_callback() {}
            virtual void mouse_down_callback() {}


            void set_layout(Table *table);

            bool has_element();

        private: 
            Table *table;
    };
}}

#endif