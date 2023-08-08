// Examples (uncomment any of these)
/* Example 1: Displays a blue window on startup. */
/*
#include <Icosahedron/Core.h>

class Example1 : public ic::Application {
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
    Example1 application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/


/* Example2: Displays a textured rectangle that can be moved around using the WASD keys or the arrow keys. */
/*
#include <Icosahedron/Core.h>

class Example2 : public ic::Application {
    ic::Batch2D *batch;
    ic::Texture<ic::T2D> *texture;
    ic::Shader *shader;

    ic::RectangleShape *shape;
    
    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }
        
        bool load() override {
            batch = new ic::Batch2D(1000, ic::TRIANGLES);
            shape = new ic::RectangleShape();
            texture = new ic::Texture<ic::T2D>({"resources/textures/wood.png"});
            shader = shaders.basicTextureShader2D;

            inputHandler.add_input(new ic::WASDController(), "WASD");

            return true;
        }

        bool handle_event(ic::Event event, float dt) override {
            return true;
        }
    
        bool update(float dt) override { 
            float speed = 1.0f;
            ic::KeyboardController *controller = (ic::KeyboardController*) inputHandler.find_input("WASD");
            ic::Vec2i dir = controller->direction;

            shape->r.position.x() += dir.x() * speed * dt;
            shape->r.position.y() += dir.y() * speed * dt;


            clear_color(ic::Colors::blue);

            shader->use();
            texture->use();
            shape->draw(renderer, batch, ic::Colors::white);
            batch->render();

            return true; 
        }

        void dispose() override {
            batch->dispose();
            texture->dispose();
            shader->clear();
        }
};

int main(int argc, char *argv[]) {
    Example2 application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/

/* Example3: A recreation of pong. The left paddle uses WS keys and the right one uses up-down keys. */
/*
#include <Icosahedron/Core.h>

class Example3 : public ic::Application {
    ic::Batch2D *batch, *textBatch;
    ic::TextureAtlas *texture;
    ic::TextAtlas *atlas;
    ic::Shader *shader, *textShader;

    ic::RectangleShape *paddle1, *paddle2, *ball;
    ic::Vec2f vel;
    int points1 = 0, points2 = 0;

    public:
        bool init() override {
            displayName = "Example window";
            return true;
        }
        
        bool load() override {
            shader = shaders.basicTextureShader2D;
            textShader = shaders.basicTextShader2D;

            // We use the arial font
            ic::FreeType::get().add_atlas("score", "C:/Windows/Fonts/arial.ttf", 48);
            atlas = ic::FreeType::get().find_atlas("score");

            texture = new ic::TextureAtlas();
            texture->add_entries({ "paddle1", "resources/textures/white.png",
                                   "paddle2", "resources/textures/white.png",
                                   "ball", "resources/textures/ball.png" });

            batch = new ic::Batch2D(1000, ic::TRIANGLES);
            textBatch = new ic::Batch2D(1000, ic::TRIANGLES);
            
            paddle1 = new ic::RectangleShape({ -0.7f, 0.0f }, { 0.05f, 0.2f }, "paddle1");
            paddle2 = new ic::RectangleShape({ 0.7f, 0.0f }, { 0.05f, 0.2f }, "paddle2");
            ball = new ic::RectangleShape({ 0.0f, 0.0f }, { 0.05f, 0.05f }, "ball");

            paddle1->set_atlas(texture);
            paddle2->set_atlas(texture);
            ball->set_atlas(texture);

            restart(nullptr);

            ic::KeyboardController *cont1 = new ic::KeyboardController();
            cont1->add_action([](ic::KeyboardController *c) {c->direction.y() = 1;}, KEY_W);
            cont1->add_action([](ic::KeyboardController *c) {c->direction.y() = -1;}, KEY_S);

            
            ic::KeyboardController *cont2 = new ic::KeyboardController();
            cont2->add_action([](ic::KeyboardController *c) {c->direction.y() = 1;}, KEY_UP);
            cont2->add_action([](ic::KeyboardController *c) {c->direction.y() = -1;}, KEY_DOWN);
            

            inputHandler.add_input(cont1, "paddle1");
            inputHandler.add_input(cont2, "paddle2");

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
            ic::KeyboardController *controller1 = (ic::KeyboardController*) inputHandler.find_input("paddle1");
            ic::KeyboardController *controller2 = (ic::KeyboardController*) inputHandler.find_input("paddle2");
            
            ic::Vec2i dir1 = controller1->direction;
            ic::Vec2i dir2 = controller2->direction;

            
            paddle1->r.position.y() += dir1.y() * speed * dt;
            paddle2->r.position.y() += dir2.y() * speed * dt;

            paddle1->r.position.clamp({ -1.0f, -1.0f + paddle1->r.size.y() }, { 1.0f, 1.0f - paddle1->r.size.y() });
            paddle2->r.position.clamp({ -1.0f, -1.0f + paddle2->r.size.y() }, { 1.0f, 1.0f - paddle2->r.size.y() });
            
            // Rendering
            clear_color(ic::Colors::black);

            shader->use();
            texture->use();
            paddle1->draw(renderer, batch, ic::Colors::white);
            paddle2->draw(renderer, batch, ic::Colors::white);
            ball->draw(renderer, batch, ic::Colors::white);
            batch->render();

            // Text rendering
            textShader->use();
            atlas->use();
            renderer.draw_string_centered(textBatch, atlas, std::to_string(points1) + " | " + std::to_string(points2), 0.0f, 0.7f);
            textBatch->render();

            return true; 
        }

        void dispose() override {
            batch->dispose();
            textBatch->dispose();
            texture->dispose();
            shader->clear();
        }

        void restart(ic::RectangleShape *winner) {
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

int main(int argc, char *argv[]) {
    Example3 application;
    
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/