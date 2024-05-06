#include <Icosahedron/Application.h>

#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/Debug.h>

#include <UI/Core.h>
#include <UI/Label.h>
#include <UI/Button.h>
#include <UI/style/TextureDrawable.h>


class UIBasics : public ic::Application {
    ic::UI::TextureDrawable *buttonDrawable;
    ic::UI::TextureDrawable *tinted;
    public:
        bool load() override {
            ic::GL::enable_blending(ic::SRC_ALPHA, ic::DEST_ONE_MINUS_SRC_ALPHA);

            static auto &ui = ic::UI::Core::get();
            ui.load();
            ui.atlas->add_entry("wood", "resources/textures/wood.png");
            ui.atlas->add_entry("ball", "resources/textures/ball.png");
            ui.atlas->add_entry("white", "resources/textures/white.png");

            srand(time(NULL));

            buttonDrawable = new ic::UI::TextureDrawable("wood");
            tinted = new ic::UI::TextureDrawable("white");
            tinted->tint = ic::Colors::black;
            tinted->tint.a = 150.0f;
            

            ui.mainTable->text_button("Show dialog", [=]() { add_table(); })
                ->set_background(buttonDrawable)
                ->set_position(0.0f, -0.7f)
                ->set_size(0.3f, 0.1f);

            
            return true;
        }

        bool update() override {
            ic::GL::clear_color(ic::Colors::blue);

            ic::UI::Core::get().update_and_render(ic::Time::globalDelta);

            return true; 
        }

        void dispose() override {
            ic::UI::Core::get().dispose();
        }

        void add_table() {
            std::cout << "Added table" << "\n";

            static auto &ui = ic::UI::Core::get();
            
            ic::UI::Table *table = new ic::UI::Table(tinted);

            ic::UI::SliderStyle style(new ic::UI::TextureDrawable("white"), new ic::UI::TextureDrawable("white"));

            table->label("Test dialog title")->set_position(0.0f, 0.5f);
            table->slider()->set_style(style)->set_position(0.0f, 0.0f);

            table->text_button("Remove", [table]() {
                ui.mainTable->remove(table);
            })->set_background(buttonDrawable)->set_position(0.0f, -0.5f);

            table->set_position(0.0f, -0.3f);
            
            ui.mainTable->add(table);
        }
};

int main() {
    ic::create_console();

    UIBasics application;

    application.construct("UI basics", 640, 480);
    application.start();

    return 0;
}