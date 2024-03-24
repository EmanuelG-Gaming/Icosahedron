#ifndef IC_UI_TABLE_H
#define IC_UI_TABLE_H

#include <vector>
#include <functional>
#include <string>

#include <UI/layout/Cell.h>
#include <UI/style/Drawable.h>
#include <UI/Label.h>
#include <UI/ImageElement.h>


namespace ic { namespace UI {
    class Button;

    /** @brief A table may also be a branch of the UI tree, with the root of it also being a table. */
    class Table {
        public:
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
            
            void update(float dt);
            
            void recalculate_size();
            

            void set_layout(Table *table);

            Table *set_background(Drawable *drawable);
            bool has_background();

            Table *set_position(float x, float y);
            Table *set_position(ic::Vec2f &pos);

            /** @brief Forcefully sets the table's width and height, without automatic scaling. */
            Table *set_size(float w, float h);
            Table *set_size(ic::Vec2f &size);

            /** @brief UI elements notation shorthands. */

            Cell *add(Element *element);
            Cell *add(Cell *cell);
            Table *add(Table *table);

            void remove(Cell *cell);
            void remove(Table *table);

            Table *table(const std::function<void(Table*)> &consumer);

            Label *label(const std::string &text);
            ImageElement *image(const std::string &atlasEntryName, float width = 0.1f, float height = 0.1f);


            Button *button();
            Button *button(const std::function<void()> &clicked);

            Button *image_button(const std::string &atlasEntryName, const std::function<void()> &clicked = nullptr);
            Button *image_button(const std::string &atlasEntryName, float width, float height, const std::function<void()> &clicked = nullptr);

            Button *text_button(const std::string &text, const std::function<void()> &clicked= nullptr);

        
        protected:
            void draw_elements();
            
        protected:
            ic::Vec2f position;
            ic::Vec2f rectanglePosition;

            float width, height;
            float prefWidth, prefHeight;
            Drawable *background;

            Table *previous;

            std::vector<Cell*> cells;
            std::vector<Table*> tables;
    };
}}

#endif