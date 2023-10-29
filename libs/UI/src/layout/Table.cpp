#include <UI/layout/Table.h>

using namespace ic::UI;

Table::Table() {
    this->background = nullptr;
}

void ic::UI::Table::draw() {
    if (this->has_background()) {
        this->background->draw(this->position.x(), this->position.y(), 0.1f, 0.1f);
    }

    for (auto &cell : this->cells) {
        if (cell->has_element()) {
            cell->element->draw();
        }
    }

    for (auto &table : this->tables) {
        table->draw();
    }
}



ic::UI::Cell *ic::UI::Table::add(Element *element) {
    ic::UI::Cell *cell = new ic::UI::Cell();
    cell->set_layout(this);
    cell->element = element;
    this->cells.push_back(cell);

    return cell;
}

ic::UI::Table *ic::UI::Table::add(Table *table) {
    this->tables.push_back(table);
    
    return table;
}



ic::UI::Table *ic::UI::Table::set_background(Drawable *background) {
    this->background = background;

    return this;
}

bool ic::UI::Table::has_background() {
    return (this->background != nullptr);
}