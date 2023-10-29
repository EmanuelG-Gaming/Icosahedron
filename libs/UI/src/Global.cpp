#include <UI/Global.h>


void ic::UI::Global::load() {
    this->fillBatch = new ic::Batch(10000, ic::TRIANGLES);
    
    ic::FreeType::get().add_atlas("default", "resources/fonts/Roboto-Regular.ttf", 48);
    this->defaultAtlas = ic::FreeType::get().find_atlas("default");
}