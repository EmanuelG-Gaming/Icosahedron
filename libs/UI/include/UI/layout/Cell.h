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
            Cell(ic::UI::Element *elem);

            virtual void mouse_moved_callback() {}
            virtual void mouse_up_callback() {}
            virtual void mouse_down_callback() {}


            void set_layout(Table *table);

            bool has_element();

        private: 
            // The inheritor table of this cell
            Table *table;
    };
}}

#endif