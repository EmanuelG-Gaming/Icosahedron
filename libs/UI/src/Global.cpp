#include <UI/Global.h>

void ic::UI::Global::load() {
    fillBatch = ic::Batch(10000, ic::TRIANGLES);
    fillTextBatch = ic::Batch(10000, ic::TRIANGLES);
    
    ic::FreeType::add_atlas("default", "resources/fonts/Roboto-Regular.ttf", 48);
    defaultAtlas = ic::FreeType::find_atlas("default");

    atlas = ic::TextureAtlas(2048, 2048);
}
