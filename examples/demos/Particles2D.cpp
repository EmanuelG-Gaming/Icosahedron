#include <Icosahedron/Application.h>

#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Batch.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Shaders.h>

#include <Icosahedron/scene/2d/Camera2D.h>

#include <Icosahedron/math/geom/Vectors.h>

#include <vector>
#include <algorithm>


struct Particle {
    ic::Vec2f position;
    ic::Vec2f velocity;
    float timeElapsed;
};

// It takes a second for a particle to dissapear
const float PARTICLE_DISSAPEAR_TIME = 1.0f;

/** Particle effects in two dimensions. */
class Particles2D : public ic::Application {
    std::vector<Particle*> particles;
    ic::Batch particleBatch;
    ic::Camera2D camera;
    ic::Shader particleShader;
    ic::Vec2f mousePreviousPosition, mouseVelocity;
    ic::Texture circleTexture;

    public:
        bool init() override {
            displayName = "Particles 2D";

            return true;
        }
        
        bool load() override {
            particleBatch = ic::Batch(100000, ic::TRIANGLES);
            particleShader = ic::Shader(shaders.basicShaderVertex2D, shaders.basicTextureShaderFrag2D);
            circleTexture = ic::TextureLoader::get().load_png("resources/textures/ball.png");

            camera = ic::Camera2D(0.5f);

            ic::MouseController *controller = new ic::MouseController();
            controller->add_mouse_down_action([this]() { 
                ic::Vec2i p = ic::InputHandler::get().find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };

                ic::Vec2f levelPos = camera.unproject(pos);

                mousePreviousPosition = levelPos;
            });

            controller->add_mouse_hold_action([this]() { 
                ic::Vec2i p = ic::InputHandler::get().find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };

                ic::Vec2f levelPos = camera.unproject(pos);

                mouseVelocity = levelPos - mousePreviousPosition;
                add_effect(levelPos, mouseVelocity);

                mousePreviousPosition = levelPos;
            });

            ic::InputHandler::get().add_input(controller, "mouse");


            return true;
        }

        void window_size_changed(int w, int h) override {
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            for (auto &particle : particles) {
                particle->position += particle->velocity * dt;
                particle->timeElapsed += dt;
            }

            particles.erase(std::remove_if(particles.begin(), particles.end(), [&](Particle *particle) { return (particle->timeElapsed >= PARTICLE_DISSAPEAR_TIME); }), particles.end());


            clear_color(ic::Colors::blue);

            particleShader.use();
            camera.use(particleShader);

            circleTexture.use();
            for (auto &particle : particles) {
                ic::Vec2f pos = particle->position;
                float from = 0.1f, to = 0.0f;
                float alpha = particle->timeElapsed / PARTICLE_DISSAPEAR_TIME;
                float interpolated = ic::Mathf::get().interpolate(from, to, sqrt(alpha));

                renderer.draw_rectangle(particleBatch, pos.x(), pos.y(), interpolated, interpolated, ic::Colors::yellow);
            }

            particleBatch.render();

            return true; 
        }

        void dispose() override {
            particleShader.clear();
            particleBatch.dispose();
            circleTexture.dispose();
        }


        void add_particle(ic::Vec2f position, ic::Vec2f velocity) {
            Particle *particle = new Particle();
            particle->position = position;
            particle->velocity = velocity;
            particle->timeElapsed = 0.0f;

            particles.push_back(particle);
        }

        void add_effect(ic::Vec2f position, ic::Vec2f velocity) {
            for (int i = 0; i < 15; i++) {
                float angle = (rand() % 1000 / 1000.0f) * 2.0f * M_PI;
                float magnitude = rand() % 30 / 30.0f;

                ic::Vec2f radialVelocity = ic::Vec2f(cos(angle), sin(angle));
                ic::Vec2f linearVelocity = velocity.nor();
                
                ic::Vec2f totalVelocity = radialVelocity * magnitude + (velocity * 10.0f * (radialVelocity.dot(linearVelocity) + 1.0f) / 2.0f);
                add_particle(position, totalVelocity);
            }
        }
};

int main() {
    Particles2D application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}