#include <Icosahedron/Application.h>
#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Texture.h>

#include <Icosahedron/util/GeometryGenerator.h>
#include <Icosahedron/scene/Mesh.h>
#include <Icosahedron/scene/2d/Camera2D.h>
#include <Icosahedron/util/IO.h>
#include <Icosahedron/Debug.h>

struct Boid {
    ic::Vec2 position;
    ic::Vec2 velocity;

    float cohesionDetectionRadius, separationDetectionRadius, alignmentDetectionRadius;
};


std::vector<Boid*> boids;
ic::Mesh boidMesh;
ic::Shader boidShader;
ic::Camera2D camera;
ic::Texture boidTexture;

float cohesionStrength = 1.0f;
float separationStrength = 0.15f;
float alignmentStrength = 0.2f;

void integrate_motion(Boid *boid, float dt) {
    boid->position = boid->position + boid->velocity * dt;
}


std::vector<Boid*> get_boids_inside_circle(ic::Vec2 origin, Boid *target, float radius) {
    std::vector<Boid*> result;

    float radiusSquared = radius * radius;

    for (auto &boid : boids) {
        if ((boid->position.dst2(origin) <= radiusSquared) && (boid != target)) {
            result.push_back(boid);
        }
    }

    return result;
}




void update_boid(Boid *boid, float dt) {
    std::vector<Boid*> insideCohesion = get_boids_inside_circle(boid->position, boid, boid->cohesionDetectionRadius);
    std::vector<Boid*> insideSeparation = get_boids_inside_circle(boid->position, boid, boid->separationDetectionRadius);
    std::vector<Boid*> insideAlignment = get_boids_inside_circle(boid->position, boid, boid->alignmentDetectionRadius);

    ic::Vec2 cohesion;
    ic::Vec2 separation;
    ic::Vec2 alignment;

    // Cohesion
    for (auto &b : insideCohesion) {
        cohesion = cohesion + b->position;
    }

    // Separation
    for (auto &b : insideSeparation) {
        separation = separation + (b->position - boid->position);
    }
    separation = separation * -1.0f;

    // Alignment
    for (auto &b : insideAlignment) {
        alignment = alignment + boid->velocity;
    }

    if (!insideCohesion.empty()) {
        cohesion = cohesion / (float) insideCohesion.size();
    }

    if (!insideAlignment.empty()) {
        alignment = alignment / (float) insideAlignment.size();
    }



    if (!insideCohesion.empty()) {
        boid->velocity = boid->velocity + ((cohesion - boid->position) * dt * cohesionStrength) * 0.5f;
    }

    if (!insideSeparation.empty()) {
        boid->velocity = boid->velocity + ((separation) * dt * separationStrength) * 0.5f;
    }

    if (!insideSeparation.empty()) {
        boid->velocity = boid->velocity + ((alignment) * dt * alignmentStrength) * 0.5f;
    }



    integrate_motion(boid, dt);
}



void init() {
    ic::create_console();

    srand(time(NULL));

    boidMesh = ic::Mesh();
    boidMesh.add_attribute(0, std::vector<ic::Vec2>({ { -0.05f, -0.05f }, { 0.1f, 0.0f }, { -0.05f, 0.05f } }));
    boidMesh.set_index_buffer({ 0, 1, 2 });

    boidTexture = ic::TextureLoader::load_png("resources/textures/white.png");

    boidShader = ic::Shader(ic::Shaders::meshShaderVertex2D, ic::Shaders::meshShaderFrag2D);
    boidShader.use();
    boidTexture.use();
    boidShader.set_uniform_float("material.colorBlending", 1.0f);
    boidShader.set_uniform_vec3f("material.baseColor", { 0.5f, 0.5f, 0.5f });


    camera = ic::Camera2D(0.25f);

    float w = 2.0f;
    float h = 2.0f;

    for (int i = 0; i < 250; i++) {
        float x = (rand() % 100 / 100.0f - 0.5f) * w;
        float y = (rand() % 100 / 100.0f - 0.5f) * h;
        float angle = rand() % 360 / 360.0f * M_PI * 2.0f;

        boids.push_back(new Boid({ ic::Vec2(x, y), ic::Vec2(cos(angle), sin(angle)) * 0.05f, 0.5f, 0.1f, 2.0f }));
    }


    ic::IO::open("tmp.txt");
    ic::IO::write(boids[0]->separationDetectionRadius);
    ic::IO::read(boids[0]->separationDetectionRadius);

    std::cout << "Size of Boid: " << sizeof(Boid) << "\n";

    ic::IO::close();


}


void update(float dt) {
    // Average value between

    for (auto &boid : boids) {
        update_boid(boid, dt);
    }
}


void render(float dt) {
    ic::GL::clear_color(ic::Colors::blue);
        

    boidShader.use();
    camera.use(boidShader);

    for (auto &boid : boids) {
        ic::Vec2 to = boid->velocity;

        boidMesh.set_transformation(ic::Mat4().set_translation<2>(boid->position) * ic::Mat4().set_rotation(atan2(to.y(), to.x())));
        boidMesh.apply_transformations(boidShader);
        boidMesh.draw();
    }
}



int main() {

    ic::Engine engine;

    engine.construct_window();
    engine.create_GL_context();
    
    engine.window.set("Boids", 640, 480);
    engine.window.set_vsync(1);

    init();

    bool enabled = true;
    ic::Event event;

    while (enabled) {
        while (engine.poll_events(event)) {
            if (!engine.process_window_callbacks(event)) {
                enabled = false;
                break;
            }
        }
        ic::InputHandler::update();

        update(ic::Time::delta);
        render(ic::Time::delta);

        engine.window.swap_buffers();
	    engine.tick();
    }

    return 0;
}
