#include <Icosahedron/Core.h>

#include <Physics/2d/levels/PhysicsLevel2D.h>
#include <Physics/2d/objects/RigidObject2D.h>

struct PointCharge : public ic::Physics::RigidObject2D {
    public:
        float charge;
        float radius;

        PointCharge(float charge, float radius) {
            this->charge = charge;
            this->radius = radius;

            this->collider = new ic::Physics::CircleCollider(this->radius);
            this->dynamic = true;
        }
};

const std::size_t fieldWidth = 50, fieldHeight = 50;

/** @brief Simulates point charges, electric fields. */
class Electrostatics : public ic::Application {
    ic::Batch *batch, *vectorFieldBatch, *textBatch, *worldTextBatch;
    ic::TextAtlas *atlas;
    ic::Texture *circleTexture;

    ic::Camera2D *camera, *uiCamera;
    ic::Shader *shader, *vectorFieldShader, *textShader;
    ic::Physics::PhysicsLevel2D *level;

    std::vector<PointCharge*> pointCharges;

    float kineticEnergy;
    bool hasVectorField;
    float vectorFieldRefreshTime;

    std::array<ic::Vec2f, fieldWidth * fieldHeight> electrostaticVectors;

    public:
        bool init() override {
            displayName = "Electrostatics";
            scaling = ic::WindowScaling::resizeable;

            return true;
        }
        
        bool load() override {
            shader = ic::ShaderLoader::get().load(shaders.basicTextureShaderVertex2D, shaders.basicTextureShaderFrag2D);
            vectorFieldShader = ic::ShaderLoader::get().load(shaders.basicShaderVertex2D, shaders.basicShaderFrag2D);
            textShader = ic::ShaderLoader::get().load(shaders.basicTextShaderVertex2D, shaders.basicTextShaderFrag2D);
            
            batch = new ic::Batch(10000, ic::TRIANGLES);
            vectorFieldBatch = new ic::Batch(100000, ic::LINES);
            textBatch = new ic::Batch(10000, ic::TRIANGLES);
            worldTextBatch = new ic::Batch(10000, ic::TRIANGLES);

            ic::FreeType::get().add_atlas("score", "resources/fonts/Roboto-Regular.ttf", 48);
            atlas = ic::FreeType::get().find_atlas("score");
            
            circleTexture = ic::TextureLoader::get().load_png("resources/textures/ball.png");
            
            camera = new ic::Camera2D();
            camera->scale = 0.5f;
            uiCamera = new ic::Camera2D();



            level = new ic::Physics::PhysicsLevel2D();
            level->set_gravity(0.0f, 0.0f);
            level->set_fixed_time_length(20);

            kineticEnergy = 0.0f;
            hasVectorField = false;
            vectorFieldRefreshTime = 0.0f;

            ic::MouseController *controller = new ic::MouseController();
            controller->add_mouse_scroll_up_action([this]() { 
                float p = inputHandler.find_mouse("mouse")->get_wheel_direction() * 0.1f;
                camera->scale = std::max(0.1f, std::min(camera->scale + p, 4.0f));
            });
            controller->add_mouse_scroll_down_action([this]() { 
                float p = inputHandler.find_mouse("mouse")->get_wheel_direction() * 0.1f;
                camera->scale = std::max(0.1f, std::min(camera->scale + p, 4.0f));
            });

            inputHandler.add_input(controller, "mouse");


            ic::KeyboardController *keyboard = (new ic::KeyboardController())->with_WASD();
            keyboard->add_key_up_action([this]() {
                hasVectorField = !hasVectorField;
            }, KEY_T);


            // Neutrons
            keyboard->add_key_up_action([this]() {
                ic::Vec2i p = inputHandler.find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };

                ic::Vec2f levelPos = camera->unproject(pos);

                add_object(levelPos.x(), levelPos.y(), 0.0f, 0.0f, 0.0f, 1.836f * 1.008f); 
            }, KEY_N);

            // Protons
            keyboard->add_key_up_action([this]() {
                ic::Vec2i p = inputHandler.find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };

                ic::Vec2f levelPos = camera->unproject(pos);

                add_object(levelPos.x(), levelPos.y(), 0.0f, 0.0f, 1.0f, 1.836f); 
            }, KEY_P);

            // Electrons
            keyboard->add_key_up_action([this]() {
                ic::Vec2i p = inputHandler.find_mouse("mouse")->get_cursor_position();
                ic::Vec2f pos = { p.x() * 1.0f, p.y() * 1.0f };

                ic::Vec2f levelPos = camera->unproject(pos);

                add_object(levelPos.x(), levelPos.y(), 0.0f, 0.0f, -1.0f, 1.0f); 
            }, KEY_E);

            inputHandler.add_input(keyboard, "keyboard");

            
            return true;
        }

        void window_size_changed(int w, int h) override {
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            if (hasVectorField) {
                vectorFieldRefreshTime += dt;
            }

            auto *controller = inputHandler.find_keyboard("keyboard");
            ic::Vec2i dir = controller->get_direction();

            float speed = 1.0f;
            camera->position.x() += dir.x() * speed * dt;
            camera->position.y() += dir.y() * speed * dt;

            // Don't reset forces late
            level->reset_forces();

            // Electrostatic dynamics
            for (auto &objectA : pointCharges) {
                for (auto &objectB : pointCharges) {
                    if (objectA == objectB) continue;

                    ic::Vec2f BtoA = objectB->transform->position - objectA->transform->position;
                    float length2 = BtoA.len2();
                    if (length2 < (objectA->radius + objectB->radius) * (objectA->radius + objectB->radius)) continue;

                    BtoA = BtoA.nor();

                    // Coulomb's law
                    float electrostaticMagnitude = (objectA->charge * objectB->charge) / (2 * ic::Mathf::get().twoPi * length2);

                    // Newton's gravity equation
                    float gravityMagnitude = -0.005f * (objectA->mass * objectB->mass) / (length2);

                    ic::Vec2f electrostaticForce = BtoA * electrostaticMagnitude;
                    ic::Vec2f gravityForce = BtoA * gravityMagnitude;

                    objectA->force = objectA->force - (electrostaticForce + gravityForce);
                }
            }

            if (vectorFieldRefreshTime >= 0.5f) {
                vectorFieldRefreshTime = 0.0f;

                for (int j = 0; j < fieldHeight; j++) {
                    for (int i = 0; i < fieldWidth; i++) {
                        ic::Vec2f &v = electrostaticVectors[j * fieldWidth + i];
                        v = { 0.0f, 0.0f };
    
                        float x = i * 0.1f - 2.0f;
                        float y = j * 0.1f - 2.0f;
    
                        for (auto &object : pointCharges) {
                            ic::Vec2f BtoA = object->transform->position - ic::Vec2f({ x * 1.0f, y * 1.0f });
                            
                            float length2 = BtoA.len2();
                            if (length2 < 0.65f) {
                                length2 = 0.65f;
                            }
        
                            BtoA = BtoA.nor();
        
                            // Coulomb's law
                            float electrostaticMagnitude = -object->charge / (2 * ic::Mathf::get().twoPi * length2);
        
                            // Newton's gravity equation
                            float gravityMagnitude = 0.005f * object->mass / (length2);
        
                            ic::Vec2f electrostaticForce = BtoA * electrostaticMagnitude;
                            ic::Vec2f gravityForce = BtoA * gravityMagnitude;
    
                            v = v + (electrostaticForce + gravityForce);
                        }
                    }
                }
            }

            level->update(dt);

            kineticEnergy = 0.0f;

            for (auto &object : level->get_objects()) {
                ic::Physics::RigidObject2D *body = dynamic_cast<ic::Physics::RigidObject2D*>(object);
                if (body == nullptr) continue;

                // Calculate kinetic energy based on the formula mv^2 / 2
                kineticEnergy += (body->mass * body->velocity.len2()) / 2.0f;
            }

            int size = level->get_objects().size();

            clear_color(ic::Colors::blue);

            // Vector fields
            vectorFieldShader->use();
            camera->use(vectorFieldShader);

            if (hasVectorField) {
                for (int j = 0; j < fieldHeight; j++) {
                    for (int i = 0; i < fieldWidth; i++) {
                        ic::Vec2f v = electrostaticVectors[j * fieldWidth + i];
                        
                        float x = i * 0.1f - 2.0f;
                        float y = j * 0.1f - 2.0f;

                        float length = v.len();
                        ic::Vec2f perpendicular = v.perpendicular(0);
                        perpendicular = perpendicular.nor() * 0.1f * length;

                        ic::Vec3f c = { length, length, length };
                        c = (c * 5.0f).clamp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
                        c = c * 255.0f;

                        ic::Color color = ic::Color(c);

                        renderer.draw_line(vectorFieldBatch, x, y, x + v.x(), y + v.y(), color);
                        renderer.draw_line(vectorFieldBatch, x + v.x(), y + v.y(), x + v.x() * 0.9f - perpendicular.x(), y + v.y() * 0.9f - perpendicular.y(), color);
                        renderer.draw_line(vectorFieldBatch, x + v.x(), y + v.y(), x + v.x() * 0.9f + perpendicular.x(), y + v.y() * 0.9f + perpendicular.y(), color);
                    }
                }
                vectorFieldBatch->render();
            }

            shader->use();
            camera->use(shader);
            circleTexture->use();

            for (auto &object : pointCharges) {
                ic::Vec2f pos = object->transform->position;

                if (object->charge < 0.0f) {
                    renderer.draw_rectangle(batch, pos.x(), pos.y(), object->radius, object->radius, ic::Colors::green);
                    renderer.draw_string_centered(worldTextBatch, atlas, "-", pos.x(), pos.y() - 0.05f, 2.0f, 2.0f);
                } else if (object->charge > 0.0f) {
                    renderer.draw_rectangle(batch, pos.x(), pos.y(), object->radius, object->radius, ic::Colors::yellow);
                    renderer.draw_string_centered(worldTextBatch, atlas, "+", pos.x(), pos.y(), 2.0f, 2.0f);
                } else {
                    renderer.draw_rectangle(batch, pos.x(), pos.y(), object->radius, object->radius, ic::Colors::lightGray);
                    renderer.draw_string_centered(worldTextBatch, atlas, "0", pos.x(), pos.y(), 1.5f, 1.5f);
                }
            }

            batch->render();
            circleTexture->unuse();


            // Text rendering
            textShader->use();
            atlas->use();

            camera->use(textShader);
            worldTextBatch->render();

            uiCamera->use(textShader);

            renderer.draw_string(textBatch, atlas, "Object count: " + std::to_string(size), -1.2f, 0.9f);
            renderer.draw_string(textBatch, atlas, "Total kinetic energy: " + std::to_string(kineticEnergy) + " J", -1.2f, 0.8f);
            renderer.draw_string(textBatch, atlas, "Press T to toggle electrostatic vector field,", -1.2f, 0.7f, 0.8f, 0.8f);
            renderer.draw_string(textBatch, atlas, "and N, P, E for adding neutrons, protons, and electrons.", -1.2f, 0.6f, 0.8f, 0.8f);
            textBatch->render();


            return true; 
        }

        void add_object(float x, float y, float velX, float velY, float chargeValue, float mass) {
            PointCharge *charge = new PointCharge(chargeValue, 0.1f);
            charge->set_mass(mass);
            //charge->isTrigger = true;

            charge->set_position(x, y);
            charge->apply_velocity(velX, velY);

            level->add_object(charge);
            pointCharges.push_back(charge);
        }

        void dispose() override {
            shader->clear();
            textShader->clear();

            batch->dispose();
            vectorFieldBatch->dispose();

            textBatch->dispose();
            worldTextBatch->dispose();
        }
};

int main(int argc, char *argv[]) {
    Electrostatics application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}