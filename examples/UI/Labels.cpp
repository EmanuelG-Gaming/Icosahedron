#include <Icosahedron/Application.h>

#include <Icosahedron/graphics/Colors.h>

#include <UI/Core.h>
#include <UI/Label.h>
#include <UI/style/TextureDrawable.h>


class Labels : public ic::Application {
    public:
        bool init() override {
            displayName = "Label test";

            return true;
        }
        
        bool load() override {
            static auto &ui = ic::UI::Core::get();
            ui.load();

            ui.atlas->add_entry("wood", "resources/textures/wood.png");

            ui.mainTable->label("Test")->set_font_color(ic::Colors::green);

            ui.mainTable->label("Text with wooden background")
                ->set_font_color(ic::Colors::yellow)
                ->set_background(new ic::UI::TextureDrawable("wood"))
                ->set_position(-0.5f, 0.4f);

            return true;
        }

        void window_size_changed(int w, int h) override {
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            clear_color(ic::Colors::blue);

            ic::UI::Core::get().update_and_render(dt);

            return true; 
        }

        void dispose() override {
            ic::UI::Core::get().dispose();
        }
};

int main(int argc, char *argv[]) {
    Labels application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}