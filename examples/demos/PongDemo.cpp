#include <Icosahedron/Application.h>
#include <Icosahedron/math/geom/Rectangle.h>
#include <Icosahedron/scene/2d/Camera2D.h>
#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/graphics/Colors.h>


struct RectangleShape {
    ic::Rectangle r;
    std::string atlasEntryName;

    RectangleShape(ic::Vec2f position, ic::Vec2f size, std::string atlasEntryName) : r(position, size), atlasEntryName(atlasEntryName) {

    }
};

/* A recreation of pong. The left paddle uses WS keys and the right one uses up-down keys. */
class PongDemo : public ic::Application {
    ic::Batch batch, textBatch;
    ic::TextureAtlas texture;
    ic::TextAtlas atlas;
    ic::Shader shader, textShader;
    ic::Camera2D camera;

    RectangleShape *paddle1, *paddle2, *ball;
    ic::Vec2f vel;
    int points1, points2;

    public:
        bool init() override {
            displayName = "Pong Demo";
            
            return true;
        }
        
        bool load() override {
            shader = ic::ShaderLoader::load(ic::Shaders::basicTextureShaderVertex2D, ic::Shaders::basicTextureShaderFrag2D);
            textShader = ic::ShaderLoader::load(ic::Shaders::basicTextShaderVertex2D, ic::Shaders::basicTextShaderFrag2D);

            // We use the roboto font
            ic::FreeType::add_atlas("score", "resources/fonts/Roboto-Regular.ttf", 48);
            atlas = ic::FreeType::find_atlas("score");

            texture = ic::TextureAtlas(2048, 2048);
            texture.add_entries({ "paddle1", "resources/textures/white.png",
                                   "paddle2", "resources/textures/white.png",
                                   "ball", "resources/textures/ball.png" });

            batch = ic::Batch(1000, ic::TRIANGLES);
            textBatch = ic::Batch(1000, ic::TRIANGLES);
            
            camera = ic::Camera2D();

            paddle1 = new RectangleShape({ -0.7f, 0.0f }, { 0.05f, 0.2f }, "paddle1");
            paddle2 = new RectangleShape({ 0.7f, 0.0f }, { 0.05f, 0.2f }, "paddle2");
            ball = new RectangleShape({ 0.0f, 0.0f }, { 0.05f, 0.05f }, "ball");

            points1 = 0;
            points2 = 0;

            restart(nullptr);

            ic::KeyboardController *cont1 = new ic::KeyboardController();
            cont1->add_action([cont1]() {cont1->direction.y() = 1;}, KEY_W);
            cont1->add_action([cont1]() {cont1->direction.y() = -1;}, KEY_S);

            
            ic::KeyboardController *cont2 = new ic::KeyboardController();
            cont2->add_action([cont2]() {cont2->direction.y() = 1;}, KEY_UP);
            cont2->add_action([cont2]() {cont2->direction.y() = -1;}, KEY_DOWN);
            

            ic::InputHandler::add_input(cont1, "paddle1");
            ic::InputHandler::add_input(cont2, "paddle2");

            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override {
            // Boundary conditions
            if (ball->r.position.y() + ball->r.size.y() > 1.0f) {
                ball->r.position.y() = 1.0f - ball->r.size.y();
                vel.y() *= -1;
            }
            if (ball->r.position.y() - ball->r.size.y() < -1.0f) {
                ball->r.position.y() = -1.0f + ball->r.size.y();
                vel.y() *= -1;
            }

            if (ball->r.position.x() + ball->r.size.x() < -1.0f) {
                restart(paddle2);
            }
            if (ball->r.position.x() - ball->r.size.x() > 1.0f) {
                restart(paddle1);
            }

            // Collision detection
            if (ball->r.overlaps(paddle1->r)) {
                ic::Vec2f overlap = ball->r.find_overlap(paddle1->r);
                if (overlap.x() < overlap.y()) {
                    ball->r.position.x() += (overlap.x() + ball->r.size.x());
                    vel.x() *= -1;
                } else {
                    int sign = (ball->r.position.y() >= paddle1->r.position.y()) ? 1 : -1;
                    ball->r.position.y() += sign * (overlap.y() + ball->r.size.y());
                    vel.y() *= -1;
                }
            }

            if (ball->r.overlaps(paddle2->r)) {
                ic::Vec2f overlap = ball->r.find_overlap(paddle2->r);
                if (overlap.x() < overlap.y()) {
                    ball->r.position.x() -= (overlap.x() + ball->r.size.x());
                    vel.x() *= -1;
                } else {
                    int sign = (ball->r.position.y() >= paddle2->r.position.y()) ? 1 : -1;
                    ball->r.position.y() += sign * (overlap.y() + ball->r.size.y());
                    vel.y() *= -1;
                }
            }

            // Dynamics
            ball->r.position.x() += vel.x() * dt;
            ball->r.position.y() += vel.y() * dt;



            float speed = 1.0f;
            auto *controller1 = ic::InputHandler::find_keyboard("paddle1");
            auto *controller2 = ic::InputHandler::find_keyboard("paddle2");
            
            ic::Vec2i dir1 = controller1->get_direction();
            ic::Vec2i dir2 = controller2->get_direction();

            
            paddle1->r.position.y() += dir1.y() * speed * dt;
            paddle2->r.position.y() += dir2.y() * speed * dt;

            paddle1->r.position = paddle1->r.position.clamp({ -1.0f, -1.0f + paddle1->r.size.y() }, { 1.0f, 1.0f - paddle1->r.size.y() });
            paddle2->r.position = paddle2->r.position.clamp({ -1.0f, -1.0f + paddle2->r.size.y() }, { 1.0f, 1.0f - paddle2->r.size.y() });
            
            // Rendering
            clear_color(ic::Colors::black);

            shader.use();
            camera.use(shader);
            texture.use();

            ic::Renderer::draw_rectangle(batch, texture.get_entry(paddle1->atlasEntryName),
                paddle1->r.position.x(), paddle1->r.position.y(), 
                paddle1->r.size.x(),     paddle1->r.size.y(), 
                ic::Colors::white);
            ic::Renderer::draw_rectangle(batch, texture.get_entry(paddle2->atlasEntryName),
                paddle2->r.position.x(), paddle2->r.position.y(), 
                paddle2->r.size.x(),     paddle2->r.size.y(), 
                ic::Colors::white);
            ic::Renderer::draw_rectangle(batch, texture.get_entry(ball->atlasEntryName),
                ball->r.position.x(), ball->r.position.y(), 
                ball->r.size.x(),     ball->r.size.y(), 
                ic::Colors::white);

            batch.render();

            // Text rendering
            textShader.use();
            camera.use(textShader);
            atlas.use();
            ic::Renderer::draw_string_centered(textBatch, atlas, std::to_string(points1) + " | " + std::to_string(points2), 0.0f, 0.7f);
            textBatch.render();

            return true; 
        }

        void dispose() override {
            batch.dispose();
            textBatch.dispose();
            texture.dispose();
            shader.clear();
        }

        void restart(RectangleShape *winner) {
            if (winner == paddle1) {
                points1++;
            } else if (winner == paddle2) {
                points2++;
            }

            paddle1->r.position = { -0.7f, 0.0f };
            paddle2->r.position = { 0.7f, 0.0f };

            ball->r.position = { 0.0f, 0.0f };
            vel = { 1.0f, 0.5f };
        }
};

int main() {
    PongDemo application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}