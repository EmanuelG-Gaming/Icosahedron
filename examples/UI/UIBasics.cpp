#include <Icosahedron/Application.h>

#include <Icosahedron/graphics/Colors.h>

#include <UI/Core.h>
#include <UI/Label.h>
#include <UI/Button.h>
#include <UI/style/TextureDrawable.h>


class UIBasics : public ic::Application {
    public:
        bool init() override {
            displayName = "UI basics";

            return true;
        }
        
        bool load() override {
            static auto &ui = ic::UI::Core::get();
            ui.load();
            ui.atlas->add_entry("wood", "resources/textures/wood.png");
            ui.atlas->add_entry("ball", "resources/textures/ball.png");
            ui.atlas->add_entry("white", "resources/textures/white.png");
            
            
            ic::UI::Button *button = ui.mainTable->text_button("Show dialog", [&]() {
                auto *tinted = new ic::UI::TextureDrawable("white");
                tinted->tint = ic::Colors::black;

                auto *table = new ic::UI::Table(tinted);

                table->label("print 'Testing'.")->set_position(-0.8f, 0.15f);
                table->label("print 'Testing'.")->set_background(new ic::UI::TextureDrawable("wood"))->set_position(-0.3f, 0.5f);
                table->label("the.")->set_background(new ic::UI::TextureDrawable("wood"))->set_position(0.5f, -0.5f);
                //table->recalculate_size();

                ui.mainTable->add(table);
            });
            button->set_background(new ic::UI::TextureDrawable("wood"));
            button->set_position(0.0f, -0.7f);

            
            return true;
        }

        void window_size_changed(int w, int h) override {
            
        }

        bool update() override {
            clear_color(ic::Colors::blue);

            ic::UI::Core::get().update_and_render(ic::Time::globalDelta);

            return true; 
        }

        void dispose() override {
            ic::UI::Core::get().dispose();
        }
};

int main() {
    UIBasics application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}