#include <UI/layout/Table.h>
#include <UI/Button.h>
#include <algorithm>

using namespace ic::UI;


Table::Table() {
    this->background = nullptr;
    this->previous = nullptr;
    this->width = 0.0f; this->prefWidth = -1.0f;
    this->height = 0.0f; this->prefHeight = -1.0f;
}

Table::Table(Drawable *background) : Table() {
    this->background = background;
}


Table::Table(const std::function<void(Table*)> &consumer) : Table() {
    consumer(this);
}

Table::Table(Drawable *background, const std::function<void(Table*)> &consumer) : Table(background) {
    consumer(this);
}



void ic::UI::Table::draw() {
    if (this->has_background()) {
        this->background->draw(this->rectanglePosition.x(), this->rectanglePosition.y(), this->width, this->height);
    }

    this->draw_elements();
}

void ic::UI::Table::draw_elements() {
    for (auto &cell : this->cells) {
        if (cell->has_element()) {
            cell->element->draw();
        }
    }

    for (auto &table : this->tables) {
        table->draw();
    }
}


void ic::UI::Table::mouse_moved() {
    this->mouse_moved_callback();

    for (auto &cell : this->cells) {
        cell->element->mouse_moved_callback();
    }

    for (auto &table : this->tables) {
        table->mouse_moved();
    }
}

void ic::UI::Table::mouse_up() {
    this->mouse_up_callback();

    for (auto &cell : this->cells) {
        cell->element->mouse_up_callback();
    }

    for (auto &table : this->tables) {
        table->mouse_up();
    }
}

void ic::UI::Table::mouse_down() {
    this->mouse_down_callback();

    for (auto &cell : this->cells) {
        cell->element->mouse_down_callback();
    }

    for (auto &table : this->tables) {
        table->mouse_down();
    }
}

void ic::UI::Table::update(float dt) {
    for (auto &table : this->tables) {
        table->update(dt);
    }
}


void ic::UI::Table::remove(ic::UI::Cell *cell) {
    if (!cell) return;

    auto iterator = std::find(cells.begin(), cells.end(), cell);
    if (iterator == cells.end()) return;

    cells.erase(iterator);
}

void ic::UI::Table::remove(ic::UI::Table *table) {
    if (!table) return;
    
    auto iterator = std::find(tables.begin(), tables.end(), table);
    if (iterator == tables.end()) return;


    tables.erase(iterator);
}


ic::UI::Cell *ic::UI::Table::add(ic::UI::Cell *cell) {
    this->cells.push_back(cell);
    //this->recalculate_size();

    return cell;
}

ic::UI::Cell *ic::UI::Table::add(Element *element) {
    ic::UI::Cell *cell = new ic::UI::Cell();
    cell->set_layout(this);
    cell->element = element;
    
    add(cell);
    
    return cell;
}

ic::UI::Table *ic::UI::Table::add(Table *table) {
    table->set_layout(this);
    this->tables.push_back(table);

    return table;
}

ic::UI::Table *ic::UI::Table::table(const std::function<void(Table*)> &consumer) {
    ic::UI::Table *table = new ic::UI::Table();
    table->set_layout(this);
    consumer(table);

    this->tables.push_back(table);
    
    return table;
}


ic::UI::Label *ic::UI::Table::label(const std::string &text) {
    ic::UI::Label *label = new ic::UI::Label(text);
    ic::UI::Cell *cell = new ic::UI::Cell();
    cell->set_layout(this);
    cell->element = label;

    add(cell);
    
    return label;
}

ic::UI::Button *ic::UI::Table::button() {
    ic::UI::Button *b = new ic::UI::Button();
    b->set_layout(this);
    this->tables.push_back(b);

    return b;
}

ic::UI::Button *ic::UI::Table::button(const std::function<void()> &clicked) {
    ic::UI::Button *b = new ic::UI::Button();
    b->set_layout(this);
    if (clicked != nullptr) {
        b->clickListener = clicked;
    }

    this->tables.push_back(b);

    return b;
}

ic::UI::Button *ic::UI::Table::image_button(const std::string &atlasEntryName, float width, float height, const std::function<void()> &clicked) {
    ic::UI::Button *b = new ic::UI::Button();
    if (clicked != nullptr) {
        b->clickListener = clicked;
    }
    b->image(atlasEntryName, width, height);

    this->tables.push_back(b);

    return b;
}

ic::UI::Button *ic::UI::Table::image_button(const std::string &atlasEntryName, const std::function<void()> &clicked) {
    return this->image_button(atlasEntryName, 0.1f, 0.1f, clicked);
}


ic::UI::Button *ic::UI::Table::text_button(const std::string &text, const std::function<void()> &clicked) {
    ic::UI::Button *b = new ic::UI::Button();
    b->set_layout(this);
    if (clicked != nullptr) {
        b->clickListener = clicked;
    }
    b->label(text);

    this->tables.push_back(b);

    return b;
}



ic::UI::ImageElement *ic::UI::Table::image(const std::string &atlasEntryName, float width, float height) {
    ic::UI::ImageElement *i = new ic::UI::ImageElement(atlasEntryName, width, height);
    ic::UI::Cell *cell = new ic::UI::Cell();

    cell->set_layout(this);
    cell->element = i;
    
    add(cell);

    return i;
}

ic::UI::Slider *ic::UI::Table::slider() {
    ic::UI::Slider *i = new ic::UI::Slider();
    ic::UI::Cell *cell = new ic::UI::Cell();

    cell->set_layout(this);
    cell->element = i;
    
    add(cell);

    return i;
}

ic::UI::Slider *ic::UI::Table::slider(const std::function<void(float)> &callback) {
    ic::UI::Slider *i = new ic::UI::Slider(callback);
    ic::UI::Cell *cell = new ic::UI::Cell();

    cell->set_layout(this);
    cell->element = i;
    
    add(cell);

    return i;
}

ic::UI::TextField *ic::UI::Table::text_field() {
    ic::UI::TextField *i = new ic::UI::TextField();
    ic::UI::Cell *cell = new ic::UI::Cell();

    cell->set_layout(this);
    cell->element = i;
    
    add(cell);

    return i;
}

ic::UI::TextField *ic::UI::Table::text_field(std::string text, TextFieldFilters filter, float width, float height, bool positiveInput,
                                            const std::function<void()> &submitCallback, const std::function<void(char)> &letterInputCallback) {
    ic::UI::TextField *i = new ic::UI::TextField(text, filter, width, height, positiveInput);
    i->submitCallback = submitCallback;
    i->letterInputCallback = letterInputCallback;
    
    ic::UI::Cell *cell = new ic::UI::Cell();

    cell->set_layout(this);
    cell->element = i;
    
    add(cell);

    return i;
}


ic::UI::Table *ic::UI::Table::set_background(Drawable *background) {
    this->background = background;

    return this;
}

bool ic::UI::Table::has_background() {
    return (this->background != nullptr);
}



ic::UI::Table *ic::UI::Table::set_position(float x, float y) {
    this->position.x() = x;
    this->position.y() = y;

    // Move cells and tables with it
    for (auto &cell : this->cells) {
        cell->element->set_position(this->position, true);
        //cell->element->translation = cell->element->relativePosition + this->position;
    }

    for (auto &table : this->tables) {
        table->set_position(this->position);
    }


    return this;
}

ic::UI::Table *ic::UI::Table::set_position(ic::Vec2f &pos) {
    return this->set_position(pos.x(), pos.y());
}

ic::UI::Table *ic::UI::Table::set_size(float w, float h) {
    this->width = this->prefWidth = w;
    this->height = this->prefHeight = h;

    return this;
}

ic::UI::Table *ic::UI::Table::set_size(ic::Vec2f &size) {
    return this->set_size(size.x(), size.y());
}



void ic::UI::Table::set_layout(ic::UI::Table *table) {
    this->previous = table;
}



void ic::UI::Table::recalculate_size() {
    for (auto &table : this->tables) {
        table->recalculate_size();
    }

    if (this->prefWidth >= 0.0f || this->prefHeight >= 0.0f) {
        this->width = this->prefWidth;
        this->height = this->prefHeight;
    } else {
        ic::Vec2f min, max;
        ic::Vec2f cellMin, cellMax;
        ic::Vec2f tableMin, tableMax;

        // First calculate at the size of cells 
        if (!cells.empty()) {
            ic::UI::Element *e = cells[0]->element;
            ic::Vec2 s(e->width, e->height);
            cellMin = e->translation - s, cellMax = e->translation + s;
    
            for (auto &cell : this->cells) {
                ic::UI::Element *elem = cell->element;
        
                float relativeMinX = elem->translation.x() - elem->width;
                float relativeMinY = elem->translation.y() - elem->height;
                float relativeMaxX = elem->translation.x() + elem->width;
                float relativeMaxY = elem->translation.y() + elem->height;
                
                if (relativeMinX < cellMin.x()) cellMin.x() = relativeMinX;
                if (relativeMinY < cellMin.y()) cellMin.y() = relativeMinY;
                if (relativeMaxX > cellMax.x()) cellMax.x() = relativeMaxX;
                if (relativeMaxY > cellMax.y()) cellMax.y() = relativeMaxY;
            }
        }


        // Then calculate the rest of the derived tables
        if (!tables.empty()) {
            ic::UI::Table *e = tables[0];
            ic::Vec2 s(e->width, e->height);
            tableMin = e->position - s, tableMax = e->position + s;
            
            for (auto &tab : this->tables) {
                ic::UI::Table *elem = tab;
        
                float relativeMinX = elem->position.x() - elem->width;
                float relativeMinY = elem->position.y() - elem->height;
                float relativeMaxX = elem->position.x() + elem->width;
                float relativeMaxY = elem->position.y() + elem->height;
                
                if (relativeMinX < tableMin.x()) tableMin.x() = relativeMinX;
                if (relativeMinY < tableMin.y()) tableMin.y() = relativeMinY;
                if (relativeMaxX > tableMax.x()) tableMax.x() = relativeMaxX;
                if (relativeMaxY > tableMax.y()) tableMax.y() = relativeMaxY;
            }
        }
        
        if (tables.empty()) {
            min = cellMin;
            max = cellMax;
        } else if (cells.empty()) {
            min = tableMin;
            max = tableMax;
        } else {
            min.x() = std::min(cellMin.x(), tableMin.x());
            min.y() = std::min(cellMin.y(), tableMin.y());
            
            max.x() = std::max(cellMax.x(), tableMax.x());
            max.y() = std::max(cellMax.y(), tableMax.y());
        }
        


        this->width = (max.x() - min.x()) / 2.0f;
        this->height = (max.y() - min.y()) / 2.0f;

        this->rectanglePosition.x() = (max.x() + min.x()) / 2.0f;
        this->rectanglePosition.y() = (max.y() + min.y()) / 2.0f;

        for (auto &table : this->tables) {
            table->recalculate_size();
        }
    }
}
