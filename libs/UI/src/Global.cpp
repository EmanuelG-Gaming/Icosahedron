#include <UI/Global.h>

void ic::UI::Global::load() {
    fillBatch = ic::Batch(10000, ic::TRIANGLES);
    fillTextBatch = ic::Batch(10000, ic::TRIANGLES);
    
    focusedTextField = nullptr;
    
    this->atlas = ic::TextureAtlas(2048, 2048);
}
