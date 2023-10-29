#include <UI/layout/Cell.h>

using namespace ic::UI;


Cell::Cell() {
    this->table = nullptr;
    this->element = nullptr;
}

void ic::UI::Cell::set_layout(ic::UI::Table *table) {
    this->table = table;
}

bool ic::UI::Cell::has_element() {
    return (this->element != nullptr);
}