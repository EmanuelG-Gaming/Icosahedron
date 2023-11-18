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

            ui.mainTable->label("'Testing'")->set_background(new ic::UI::TextureDrawable(ui.atlas.add_entry("wood", "resources/textures/stone-bricks.png")))->set_position(-0.2f, 0.1f);

            ui.mainTable->button([this]() {
                std::cout << "Testing." << "\n";
            })->set_style(ic::UI::ButtonStyle(
                nullptr,
                new ic::UI::TextureDrawable(ui.atlas.add_entry("ball", "resources/textures/ball.png")),
                new ic::UI::TextureDrawable(ui.atlas.add_entry("white", "resources/textures/white.png"))
            ))->set_background(new ic::UI::TextureDrawable(ui.atlas.add_entry("wood", "resources/textures/wood.png")));

            return true;
        }

        void window_size_changed(int w, int h) override {
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            clear_color(ic::Colors::blue);

            static auto &ui = ic::UI::Core::get();
            ui.render();

            return true; 
        }

        void dispose() override {
            static auto &ui = ic::UI::Core::get();
            ui.dispose();
        }
};

int main(int argc, char *argv[]) {
    UIBasics application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}