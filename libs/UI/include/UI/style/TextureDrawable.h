#ifndef IC_UI_TEXTURE_DRAWABLE_H
#define IC_UI_TEXTURE_DRAWABLE_H

#include <Icosahedron/graphics/gl/TextureAtlas.h>

#include <UI/Global.h>
#include <UI/style/Drawable.h>


namespace ic { namespace UI {
    class TextureDrawable : public Drawable {
        public:
            AtlasEntry entry;
            ic::Color tint;

            TextureDrawable() {
                this->tint = { 255, 255, 255 };
            }

            TextureDrawable(const AtlasEntry &entry, const ic::Color &tint = { 255, 255, 255 }) {
                this->entry = entry;
                this->tint = tint;
            }

            TextureDrawable(const std::string &atlasEntryName, const ic::Color &tint = { 255, 255, 255 }) {
                this->entry = ic::UI::Global::get().atlas.get_entry(atlasEntryName);
                this->tint = tint;
            }
            
            void draw(float x, float y, float width, float height) override {
                ic::Renderer::draw_rectangle(ic::UI::Global::get().fillBatch, entry, x, y, width, height, this->tint);
            }
    };
}}

#endif