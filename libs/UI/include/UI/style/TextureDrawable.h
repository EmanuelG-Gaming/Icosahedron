#ifndef IC_UI_TEXTURE_DRAWABLE_H
#define IC_UI_TEXTURE_DRAWABLE_H

#include <Icosahedron/graphics/gl/TextureAtlas.h>

#include <UI/Global.h>
#include <UI/style/Drawable.h>


namespace ic { namespace UI {
    class TextureDrawable : public Drawable {
        public:
            AtlasEntry entry;

            TextureDrawable() {
            }

            TextureDrawable(const AtlasEntry &entry) {
                this->entry = entry;
            }
            
            void draw(float x, float y, float width, float height) override {
                ic::UI::Global::get().renderer.draw_rectangle(ic::UI::Global::get().fillBatch, entry, x, y, width, height);
            }
    };
}}

#endif