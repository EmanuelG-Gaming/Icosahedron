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

            ui.mainTable->label("Press the button below to print 'Testing'.")->set_position(-0.8f, 0.15f);

            ui.mainTable->button([this]() {
                std::cout << "Testing." << "\n";
            })->set_style(ic::UI::ButtonStyle(
                nullptr,
                new ic::UI::TextureDrawable("ball"),
                new ic::UI::TextureDrawable("white")
            ))->set_background(new ic::UI::TextureDrawable("wood"));

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
    UIBasics application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}