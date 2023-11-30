#include <UI/layout/Table.h>
#include <UI/Button.h>

using namespace ic::UI;


Table::Table() {
    this->background = nullptr;
}

Table::Table(Drawable *background) {
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
        this->background->draw(this->position.x(), this->position.y(), 0.1f, 0.1f);
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

ic::UI::Table *ic::UI::Table::table(const std::function<void(Table*)> &consumer) {
    ic::UI::Table *table = new ic::UI::Table();
    consumer(table);
    this->tables.push_back(table);
    
    return table;
}


ic::UI::Label *ic::UI::Table::label(const std::string &text) {
    ic::UI::Label *label = new ic::UI::Label(text);
    ic::UI::Cell *cell = new ic::UI::Cell();
    cell->set_layout(this);
    cell->element = label;
    
    this->cells.push_back(cell);

    return label;
}

ic::UI::Button *ic::UI::Table::button() {
    ic::UI::Button *b = new ic::UI::Button();
    this->tables.push_back(b);

    return b;
}

ic::UI::Button *ic::UI::Table::button(const std::function<void()> &clicked) {
    ic::UI::Button *b = new ic::UI::Button();
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
    
    this->cells.push_back(cell);

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