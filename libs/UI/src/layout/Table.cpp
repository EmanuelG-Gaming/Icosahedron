#include <UI/layout/Table.h>
#include <UI/Button.h>

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
        this->background->draw(this->position.x(), this->position.y(), this->width, this->height);
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
        cell->mouse_moved_callback();
    }

    for (auto &table : this->tables) {
        table->mouse_moved();
    }
}

void ic::UI::Table::mouse_up() {
    this->mouse_up_callback();

    for (auto &cell : this->cells) {
        cell->mouse_up_callback();
    }

    for (auto &table : this->tables) {
        table->mouse_up();
    }
}

void ic::UI::Table::mouse_down() {
    this->mouse_down_callback();

    for (auto &cell : this->cells) {
        cell->mouse_down_callback();
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

    for (auto &tab : table->tables) {
        table->remove(tab);
    }
    for (auto &cell : table->cells) {
        table->remove(cell);
    }

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

    // Move tables with it
    for (auto &cell : this->cells) {
        cell->element->set_position(this->position);
    }

    for (auto &table : this->tables) {
        table->set_position(this->position);
    }


    return this;
}

ic::UI::Table *ic::UI::Table::set_position(ic::Vec2f &pos) {
    return this->set_position(pos.x(), pos.y());
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
        if (cells.empty()) return;
        
        ic::UI::Element *e = cells[0]->element;
        ic::Vec2 s(e->width, e->height);
        ic::Vec2 min = e->translation - s, max = e->translation + s;
    
        // First calculate at the size of cells
        
        for (auto &cell : this->cells) {
            ic::UI::Element *elem = cell->element;
    
            float relativeMinX = elem->translation.x() - elem->width;
            float relativeMinY = elem->translation.y() - elem->height;
    
            float relativeMaxX = elem->translation.x() + elem->width;
            float relativeMaxY = elem->translation.y() + elem->height;
            
    
            if (relativeMinX < min.x()) min.x() = relativeMinX;
            if (relativeMinY < min.y()) min.y() = relativeMinY;
    
            if (relativeMaxX > max.x()) max.x() = relativeMaxX;
            if (relativeMaxY > max.y()) max.y() = relativeMaxY;
        }
    
        this->width = (max.x() - min.x()) / 2.0f;
        this->height = (max.y() - min.y()) / 2.0f;

        this->position.x() = (max.x() + min.x()) / 2.0f;
        this->position.y() = (max.y() + min.y()) / 2.0f;
    
    

        // Then calculate the rest of the derived tables
        /*
        for (auto &tab : this->tables) {
            ic::UI::Table *elem = tab;
    
            float relativeMinX = elem->position.x() - elem->width;
            float relativeMinY = elem->position.y() - elem->height;
    
            float relativeMaxX = elem->position.x() + elem->width;
            float relativeMaxY = elem->position.y() + elem->height;
            
    
            if (relativeMinX < min.x()) min.x() = relativeMinX;
            if (relativeMinY < min.y()) min.y() = relativeMinY;
    
            if (relativeMaxX > max.x()) max.x() = relativeMaxX;
            if (relativeMaxY > max.y()) max.y() = relativeMaxY;
        }
    
        this->width = (max.x() - min.x()) / 2.0f;
        this->height = (max.y() - min.y()) / 2.0f;

        this->position.x() = (max.x() + min.x()) / 2.0f;
        this->position.y() = (max.y() + min.y()) / 2.0f;

        for (auto &table : this->tables) {
            table->recalculate_size();
        }
        */
    }
}