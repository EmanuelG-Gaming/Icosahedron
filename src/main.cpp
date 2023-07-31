// Console testing
/*
#include <iostream>
#include <Icosahedron/math/geom/Vector.h>
#include <Icosahedron/graphics/Color.h>

void vector_test() {
    ic::Vector<float, 2> vec1 = { 0.0f, 1.2f };
    ic::Vector<float, 2> vec2 = { 300.5f, 4.0f };

    ic::Vector<float, 2> result = vec1 + vec2;
    
    std::cout << "Vec1: " << vec1 << "\n";
    std::cout << "Vec2: " << vec2 << "\n";
    std::cout << "Result: " << result << "\n";
    std::cout << "Result's size: " << result.size() << "\n";
}

void color_test() {
    ic::Color c1 = ic::Color().hexadecimal_to_RGB("FF0000");
    ic::Color c2 = { 100, 255, 5 };
    ic::Color c3 = c1 * c2;

    std::cout << "Color1: " << c1 << "\n";
    std::cout << "Color2: " << c2 << "\n";
    std::cout << "Color3: " << c3 << "\n";
}

int main() {
    //vector_test();
    color_test();

    return 0;
}
*/

#include <Icosahedron/Core.h>

/* Displays a blue window on startup. */
class Example : public ic::Application {
    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }
        
        bool load() override {
            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override { 
            clear_color(ic::Colors::blue);
            return true; 
        }
};

int main(int argc, char *argv[]) {
    Example application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}