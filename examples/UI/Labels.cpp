#include <Icosahedron/Core.h>

#include <UI/Core.h>
#include <UI/Label.h>


class Labels : public ic::Application {
    public:
        bool init() override {
            displayName = "Label test";

            return true;
        }
        
        bool load() override {
            static auto &ui = ic::UI::Core::get();
            ui.load();

            ui.mainTable->add(new ic::UI::Label("Test"));

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
    Labels application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}