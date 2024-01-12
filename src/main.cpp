/* A prime example: Displays a blue window on startup. */
/*
#include <Icosahedron/Application.h>
#include <Icosahedron/graphics/Colors.h>

class FirstProject : public ic::Application {
    public:
        bool init() override {
            // Use this to set up window settings, although this can also be done in the constructor
            displayName = "Example window";

            return true;
        }
        
        bool load() override {
            // This function is called after init() and after setting up the window

            return true;
        }

        void window_size_changed(int w, int h) override {
            // Called when the window is resized, and also if it is changed to fullscreen mode
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            // Called when events are retrieved and polled 

            return true;
        }

        bool update(float dt) override {
            // This is called once every frame
        
            clear_color(ic::Colors::blue);
            return true; 
        }

        void dispose() override {
            // Occurs when the application has to close

        }
};

// A simpler main function declaration.
// You can use int argc and char *argv[] as parameters, though.
int main() {
    FirstProject application;

    // Constructs a window that is 640 pixels wide and 480 pixels tall
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/

/*
#include <Icosahedron/Application.h>

#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Shader.h>

#include <Icosahedron/scene/2d/Camera2D.h>
#include <Icosahedron/scene/2d/Mesh2D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>


class Example : public ic::Application {
    ic::Shader testShader;
    ic::Mesh2D testMesh;
    ic::Mesh2D testSquare;
    ic::Camera2D camera;

    public:
        bool init() override {
            displayName = "Example window";
            scaling = ic::WindowScaling::resizeable;
            
            return true;
        }
        
        bool load() override {
            testShader = ic::ShaderLoader::get().load(shaders.meshShaderVertex2D, shaders.meshShaderFrag2D);
            testMesh = ic::GeometryGenerator::get().generate_regular_polygon_mesh(7, 0.3f);

            testMesh.set_material(ic::MeshMaterial2D(ic::Colors::white, 1.0f));
            testMesh.set_transformation(ic::Mat4x4().set_translation<2>({ -0.5f, 0.0f }));
            
            std::vector<ic::Color> colors = {
                ic::Colors::red, 
                ic::Colors::green,
                ic::Colors::blue,
                ic::Colors::white
            };

            testSquare = ic::GeometryGenerator::get().generate_rectangle_mesh(0.2f, 0.2f);
            testSquare.add_attribute(1, 3, colors);
            testSquare.set_material(ic::MeshMaterial2D(ic::Colors::white, 0.1f));

            testSquare.set_transformation(ic::Mat4x4().set_translation<2>({ 0.5f, 0.0f }));

            camera = ic::Camera2D();

            return true;
        }

        bool update(float dt) override {
            clear_color(ic::Colors::blue);

            testShader.use();
            camera.use(testShader);
            testMesh.draw(testShader);
            testSquare.draw(testShader);

            return true; 
        }
};



int main(int argc, char *argv[]) {
    Example application;

    // Constructs a window that is 640 pixels wide and 480 pixels tall
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}

*/

#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/assets/loaders/TextureLoader.h>

#include <Icosahedron/graphics/Colors.h>
#include <Icosahedron/graphics/gl/Texture.h>

#include <Icosahedron/math/geom/Quaternion.h>

#include <Icosahedron/scene/3d/Mesh3D.h>
#include <Icosahedron/scene/3d/Camera3D.h>
#include <Icosahedron/scene/3d/controllers/FreeRoamCameraController3D.h>

#include <Icosahedron/entity/Entities.h>
#include <Icosahedron/entity/Entity.h>

#include <Physics/2d/levels/PhysicsLevel2D.h>
#include <Physics/2d/objects/RigidObject2D.h>


std::string mainSceneFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;
    in vec2 vTCoords;
    in vec3 vNormal;
    struct DirectionalLight {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

    DirectionalLight l = DirectionalLight(
        vec3(-8.0, -5.0, -2.0), 
        vec3(0.2, 0.2, 0.2), 
        vec3(0.8, 0.8, 0.8), 
        vec3(0.5, 0.5, 0.5)
    );
    
    uniform sampler2D sampleTexture;

    uniform vec3 viewPosition;
    out vec4 outColor;

    vec4 compute_lighting(DirectionalLight light) {
        vec3 normal = normalize(vNormal);
        vec3 lightDirection = normalize(-light.direction);
        vec3 viewDirection = normalize(viewPosition - vPosition);
        float dotProduct = dot(lightDirection, normal);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4;
        vec4 ambientColor = vec4(light.ambient, 1.0) * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0);
        vec4 diffuseColor = texture(sampleTexture, vTCoords);
        if (diffuseColor.a <= 0.1) diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
        else diffuseColor *= vec4(light.diffuse, 1.0) * diffuseIntensity;

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0);
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0);
        
        vec4 specularColor = vec4(light.specular, 1.0) * specularIntensity;
        vec4 result = ambientColor + diffuseColor + specularColor;
        return result;
    }
    
    void main() {
        vec4 color = compute_lighting(l);
        outColor = color;
    }
);



struct TransformComponent : public ic::Component {
    ic::Vec3f position;
    ic::Quaternion rotation;
    ic::Vec3f scaling;

    TransformComponent() {
        this->rotation = this->rotation.from_euler(0.0f, 0.0f, 0.0f);
        this->scaling = { 1.0f, 1.0f, 1.0f };
    }
    TransformComponent(ic::Vec3f position, ic::Quaternion rotation, ic::Vec3f scaling) : position(position), rotation(rotation), scaling(scaling) {}
};

struct MeshComponent : public ic::Component {
    ic::Mesh3D mesh;
    ic::Texture texture;

    MeshComponent() {}
    MeshComponent(ic::Mesh3D mesh, ic::Texture texture) : mesh(mesh), texture(texture) {}  
};

struct PhysicsComponent : public ic::Component {
    ic::Physics::RigidObject2D *rigidBody;

    PhysicsComponent(ic::Physics::Collider2D *collider, ic::Vec2f position, float angle = 0.0f, float mass = 1.0f, bool dynamic = true) {
        this->rigidBody = new ic::Physics::RigidObject2D();

        this->rigidBody->collider = collider;
        this->rigidBody->mass = mass;
        this->rigidBody->transform->angle = angle;
        this->rigidBody->dynamic = dynamic;
        this->rigidBody->restitution = 0.5f;
        this->rigidBody->set_position(position);
    }
};


struct MeshSystem {
    ic::Camera3D mainCamera;
    ic::FreeRoamCameraController3D controller;
    float time = 0.0f;

    void init() {
        mainCamera = ic::Camera3D();
        mainCamera.position = { 5.0f, 1.0f, 5.0f };
        
        controller = ic::FreeRoamCameraController3D(&mainCamera);
        controller.flying = true;
    }

    void update(ic::Entities &registry, float dt) {
        time += dt;
        controller.act(dt);
        mainCamera.update();

        for (int i = 0; i < ic::lastEntityIndex; i++) {
            ic::Entity *entity = registry.get_entity(i);

            if (entity->has<TransformComponent>() && entity->has<MeshComponent>()) {
                auto &transform = entity->get<TransformComponent>();

                ic::Mat4x4 scaling = ic::Mat4x4().set_scaling(transform.scaling);
                ic::Mat4x4 normalTransformation = transform.rotation.to_rotation_matrix();
                ic::Mat4x4 transformation = ic::Mat4x4().set_translation(transform.position) * normalTransformation * scaling;


                entity->get<MeshComponent>().mesh.set_normal_transformation(normalTransformation);
                entity->get<MeshComponent>().mesh.set_transformation(transformation);
            }
        }
    }

    void render(ic::Entities &registry, ic::Shader &shader) {
        shader.use();
        mainCamera.upload_to_shader(shader);
        shader.set_uniform_vec3f("viewPosition", mainCamera.position);

        for (int i = 0; i < ic::lastEntityIndex; i++) {
            ic::Entity *entity = registry.get_entity(i);

            if (entity->has<MeshComponent>()) {
                entity->get<MeshComponent>().texture.use();
                entity->get<MeshComponent>().mesh.draw(shader);
            }
        }
    }
};


struct PhysicsSystem {
    ic::Physics::PhysicsLevel2D level;

    void init() {
        level.set_gravity(0.0f, -9.81f);
        level.simulationSteps = 10;
    }

    void add_rigid_body(ic::Physics::RigidObject2D *rigid) {
        level.add_object(rigid);
    }

    void update(ic::Entities &registry, float dt) {
        level.update(dt);

        for (auto &object : level.get_objects()) {
            if (!object->dynamic) continue;

            ic::Physics::RigidObject2D *rigid = dynamic_cast<ic::Physics::RigidObject2D*>(object);
            if (rigid == nullptr) continue;

            if (rigid->transform->position.y() < -50.0f) {
                rigid->transform->position.y() = -50.0f;
                rigid->velocity.y() *= -1.0f;
            }

            // Very far Y bound
            if (rigid->transform->position.y() > 2000.0f) {
                rigid->transform->position.y() = 2000.0f;
                rigid->velocity.y() *= -1.0f;
            }

            if (rigid->transform->position.x() < -50.0f) {
                rigid->transform->position.x() = -50.0f;
                rigid->velocity.x() *= -1.0f;
            }

            if (rigid->transform->position.x() > 50.0f) {
                rigid->transform->position.x() = 50.0f;
                rigid->velocity.x() *= -1.0f;
            }
        }


        for (int i = 0; i < ic::lastEntityIndex; i++) {
            ic::Entity *entity = registry.get_entity(i);

            if (entity->has<TransformComponent>() && entity->has<PhysicsComponent>()) {
                ic::Vec2f pos = entity->get<PhysicsComponent>().rigidBody->transform->position;

                entity->get<TransformComponent>().position = { pos.x(), 0.2f, pos.y() };
                entity->get<TransformComponent>().rotation = ic::Quaternion().from_euler(0.0f, entity->get<PhysicsComponent>().rigidBody->transform->angle, 0.0f);
            }
        }
    }
};



class App : public ic::Application {
    ic::Entities entities;
    MeshSystem meshes;
    PhysicsSystem physics;

    ic::Shader mainSceneShader;

    public:
        bool init() override {
            displayName = "3D application";
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            states.enable_face_culling(ic::FRONT, ic::CCW);

            mainSceneShader = ic::Shader(shaders.meshShaderVertex3D, mainSceneFragment);

            meshes.init();
            physics.init();

            {
                ic::Entity *entity = entities.add_entity();
                entity->add<TransformComponent>(
                    ic::Vec3f({ 5.0f, 4.0f, 0.0f }),
                    ic::Quaternion().from_euler(0.0f, 0.0f, 0.0f),
                    ic::Vec3f({ 1.0f, 1.0f, 1.0f })
                );

                entity->add<MeshComponent>(
                    ic::GeometryGenerator::get().generate_cube_mesh(1.0f),
                    ic::TextureLoader::get().load_png("resources/textures/stone-bricks.png")
                );
            }

            // Floor
            {
                ic::Entity *entity = entities.add_entity();
                entity->add<TransformComponent>(
                    ic::Vec3f({ 0.0f, -1.0f, 0.0f }),
                    ic::Quaternion().from_euler(0.0f, 0.0f, 0.0f),
                    ic::Vec3f({ 1.0f, 1.0f, 1.0f })
                );

                entity->add<MeshComponent>(
                    ic::GeometryGenerator::get().generate_parallelipiped_mesh(50.0f, 0.1f, 50.0f, 100.0f, 0.2f, 100.0f),
                    ic::TextureLoader::get().load_png("resources/textures/grass.png")
                );
            }




            {
                ic::Entity *entity = entities.add_entity();
                entity->add<TransformComponent>();

                entity->add<MeshComponent>(
                    ic::GeometryGenerator::get().generate_UV_sphere_mesh(1.0f, 12, 12),
                    ic::TextureLoader::get().load_png("resources/textures/wood.png")
                );


                physics.add_rigid_body(
                    entity->add<PhysicsComponent>(
                        new ic::Physics::CircleCollider(1.0f),
                        ic::Vec2f({ 2.0f, 2.0f }),
                        0.0f, 2.0f
                    ).rigidBody
                );
            }


            for (int i = 0; i < 100; i++) {
                ic::Entity *entity = entities.add_entity();
                entity->add<TransformComponent>();

                entity->add<MeshComponent>(
                    ic::GeometryGenerator::get().generate_UV_sphere_mesh(0.8f, 12, 12),
                    ic::TextureLoader::get().load_png("resources/textures/white.png")
                );


                physics.add_rigid_body(
                    entity->add<PhysicsComponent>(
                        new ic::Physics::CircleCollider(0.8f),
                        ic::Vec2f({ 1.0f, 5.0f + i * 2.0f }),
                        0.0f, 2.0f
                    ).rigidBody
                );
            }

            {
                ic::Entity *entity = entities.add_entity();
                entity->add<TransformComponent>();

                entity->add<MeshComponent>(
                    ic::GeometryGenerator::get().generate_parallelipiped_mesh(10.0f, 0.8f, 5.0f, 20.0f, 1.6f, 10.0f),
                    ic::TextureLoader::get().load_png("resources/textures/stone.png")
                );


                physics.add_rigid_body(
                    entity->add<PhysicsComponent>(
                        new ic::Physics::RectangleCollider(10.0f, 5.0f),
                        ic::Vec2f({ 1.0f, 6.0f }),
                        0.5f, 2.0f, false
                    ).rigidBody
                );
            }


            {
                ic::Entity *entity = entities.add_entity();
                entity->add<TransformComponent>();

                entity->add<MeshComponent>(
                    ic::GeometryGenerator::get().generate_parallelipiped_mesh(2.0f, 0.8f, 0.5f, 4.0f, 1.6f, 1.0f),
                    ic::TextureLoader::get().load_png("resources/textures/stone.png")
                );


                physics.add_rigid_body(
                    entity->add<PhysicsComponent>(
                        new ic::Physics::RectangleCollider(2.0f, 0.5f),
                        ic::Vec2f({ 2.0f, -2.5f }),
                        0.5f, 2.0f, false
                    ).rigidBody
                );
            }


            {
                ic::Entity *entity = entities.add_entity();
                entity->add<TransformComponent>();

                entity->add<MeshComponent>(
                    ic::GeometryGenerator::get().generate_parallelipiped_mesh(2.0f, 0.8f, 0.5f, 4.0f, 1.6f, 1.0f),
                    ic::TextureLoader::get().load_png("resources/textures/stone-bricks.png")
                );

                physics.add_rigid_body(
                    entity->add<PhysicsComponent>(
                        new ic::Physics::RectangleCollider(2.0f, 0.5f),
                        ic::Vec2f({ -3.0f, 0.0f }),
                        1.0f, 2.0f, false
                    ).rigidBody
                );
            }


            {
                ic::Entity *entity = entities.add_entity();
                entity->add<TransformComponent>();

                entity->add<MeshComponent>(
                    ic::GeometryGenerator::get().generate_parallelipiped_mesh(2.0f, 0.8f, 0.5f, 4.0f, 1.6f, 1.0f),
                    ic::TextureLoader::get().load_png("resources/textures/wood.png")
                );

                physics.add_rigid_body(
                    entity->add<PhysicsComponent>(
                        new ic::Physics::RectangleCollider(2.0f, 0.5f),
                        ic::Vec2f({ -3.0f, -4.0f }),
                        0.0f, 2.0f, false
                    ).rigidBody
                );
            }

            return true;
        }

        void window_size_changed(int w, int h) override {
            
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            physics.update(entities, dt);
            meshes.update(entities, dt);

            clear_color(ic::Colors::blue);
            meshes.render(entities, mainSceneShader);

            return true; 
        }

        void dispose() override {
            
        }
};

int main(int argc, char *argv[]) {
    App application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}