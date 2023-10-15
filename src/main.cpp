/* A prime example: Displays a blue window on startup. */
/*
#include <Icosahedron/Core.h>

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

int main(int argc, char *argv[]) {
    FirstProject application;

    // Constructs a window that is 640 pixels wide and 480 pixels tall
    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}
*/




#include <Icosahedron/Core.h>

#include <Physics/2d/objects/RigidObject2D.h>
#include <Physics/2d/levels/PhysicsLevel2D.h>


std::string skyboxShaderVert = IC_ADD_GLSL_DEFINITION(
    layout (location = 0) in vec3 position;
    
    uniform mat4 view = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    uniform mat4 projection = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    
    out vec3 vPosition;
    
    void main() {
        vec4 pos = vec4(position, 1.0);
        vPosition = position;
        
        gl_Position = projection * mat4(mat3(view)) * pos;
    }
);

std::string skyboxShaderFrag = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;

    out vec4 outColor;

    uniform samplerCube skyboxSampler;

    void main() {
        outColor = texture(skyboxSampler, vPosition);
    }
);

std::string depthShaderVert = IC_ADD_GLSL_DEFINITION(
    layout (location = 0) in vec3 position;
    layout (location = 3) in vec3 normal;

    uniform mat4 lightSpaceMatrix;
    uniform mat4 model;
    uniform mat4 normalModel;

    out vec3 vPosition;
    out vec3 vNormal;
    
    void main() {
        vec4 pos = lightSpaceMatrix * model * vec4(position, 1.0);

        vPosition = pos.xyz;
        vNormal = vec3(normalModel * vec4(normal, 1.0));
        
        gl_Position = pos;
    }
);

std::string depthShaderFrag = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;
    in vec3 vNormal;

    void main() {
        vec3 normal = normalize(vNormal);
        vec3 lightDir = normalize(vec3(0.0, 0.0, 0.0) - vec3(-2.0, 3.0, -3.0));

        float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.001);
        //float bias = 0.01;

        //float depth = gl_FragCoord.z + (gl_FrontFacing ? bias : 0.0);
        float depth = gl_FragCoord.z + bias;
        gl_FragDepth = depth; 
    }
);

std::string vert = IC_ADD_GLSL_DEFINITION(
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    layout (location = 2) in vec2 tCoords;
    layout (location = 3) in vec3 normal;

    uniform mat4 projection = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    uniform mat4 view = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    uniform mat4 model = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    uniform mat4 normalModel = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    
    uniform mat4 lightSpaceMatrix;

    out vec3 vPosition;
    out vec3 vColor;
    out vec2 vTCoords;
    out vec3 vNormal;
    out vec4 vLightSpacePosition;

    void main() {
        vec4 pos = model * vec4(position, 1.0);

        vPosition = pos.xyz;
        vColor = color;
        vTCoords = tCoords;
        vNormal = vec3(normalModel * vec4(normal, 1.0));
        vLightSpacePosition = lightSpaceMatrix * pos;

        gl_Position = projection * view * pos;
    }
);

std::string fragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;
    in vec2 vTCoords;
    in vec3 vNormal;
    in vec4 vLightSpacePosition;

    struct PointLight {
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float constant;
        float linear;
        float quadratic;
    };

    struct DirectionalLight {
        vec3 direction;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

    PointLight l = PointLight(
        vec3(-5.0, 3.0, -10.0),
        
        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.8, 0.7, 0.55), 
        
        1.0, 0.09, 0.032
    );
    
    PointLight l2 = PointLight(
        vec3(5.0, 3.0, 10.0),
        
        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.8, 0.7, 0.55), 
        
        1.0, 0.09, 0.032
    );

    DirectionalLight d = DirectionalLight(
        vec3(-2.0, 3.0, -3.0), 

        vec3(0.2, 0.2, 0.4), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.1, 0.1, 0.1)
    );

    const vec4 fogColor = vec4((vec3(65.0, 89.0, 163.0) + 50.0) / 255.0, 1.0);
    const float fogIntensity = 0.0005;

    uniform sampler2D sampleTexture;
    uniform sampler2D shadowMap;
    uniform vec3 viewPosition;

    out vec4 outColor;

    float compute_shadows(PointLight light, vec4 lightSpaceCoords) {
        // Perform perspective divide
        vec3 projCoords = lightSpaceCoords.xyz / lightSpaceCoords.w;
        // Convert to [0, 1] range
        projCoords = projCoords * 0.5 + 0.5;

        vec3 normal = normalize(vNormal);
        vec3 lightDir = normalize(light.position - vPosition);
 
        float currentDepth = projCoords.z;
        
        float result = 0.0;
        // Hard shadows
        //float closestDepth = texture(shadowMap, projCoords.xy).r;
        //result = currentDepth > closestDepth ? 0.85 : 0.0;

        // PCF soft shadows
        vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
        for (int i = -1; i <= 1; i++) for (int j = -1; j <= 1; j++) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(i, j) * texelSize).r;
            result += currentDepth > pcfDepth ? 0.85 : 0.0;
        }
        result /= 9.0;

        if (projCoords.z > 1.0) result = 0.0;

        return result;
    }

    float compute_shadows(DirectionalLight light, vec4 lightSpaceCoords) {
        // Perform perspective divide
        vec3 projCoords = lightSpaceCoords.xyz / lightSpaceCoords.w;
        // Convert to [0, 1] range
        projCoords = projCoords * 0.5 + 0.5;

        vec3 normal = normalize(vNormal);
        vec3 lightDir = normalize(light.direction);
 
        float currentDepth = projCoords.z;
        
        float result = 0.0;
        // Hard shadows
        //float closestDepth = texture(shadowMap, projCoords.xy).r;
        //result = currentDepth > closestDepth ? 0.85 : 0.0;

        // PCF soft shadows
        vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
        for (int i = -1; i <= 1; i++) for (int j = -1; j <= 1; j++) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(i, j) * texelSize).r;
            result += currentDepth > pcfDepth ? 0.85 : 0.0;
        }
        result /= 9.0;

        if (projCoords.z > 1.0) result = 0.0;

        return result;
    }
    
    vec4 compute_lighting(PointLight light) {
        float distance = length(light.position - vPosition);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        vec3 normal = normalize(vNormal);
        vec3 lightDirection = normalize(light.position - vPosition);
        vec3 viewDirection = normalize(viewPosition - vPosition);
        float dotProduct = dot(lightDirection, normal);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4 * attenuation;
        vec4 ambientColor = vec4(light.ambient, 1.0) * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0) * attenuation;
        vec4 diffuseColor = texture(sampleTexture, vTCoords);
        if (diffuseColor.a <= 0.1) discard;
        else diffuseColor *= vec4(light.diffuse, 1.0) * diffuseIntensity;

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        vec4 specularColor = vec4(light.specular, 1.0) * specularIntensity;

        vec4 result = ambientColor + diffuseColor + specularColor;

        return result;
    }
    
    vec4 compute_lighting(DirectionalLight light) {
        vec3 normal = normalize(vNormal);
        vec3 lightDirection = normalize(light.direction);
        vec3 viewDirection = normalize(viewPosition - vPosition);
        float dotProduct = dot(lightDirection, normal);

        // Ambient reflection (indirect illumination approximation)
        float ambientIntensity = 0.4;
        vec4 ambientColor = vec4(light.ambient, 1.0) * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0);
        vec4 diffuseColor = texture(sampleTexture, vTCoords);
        if (diffuseColor.a <= 0.1) discard;
        else diffuseColor *= vec4(light.diffuse, 1.0) * diffuseIntensity;

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0);
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0);
        
        vec4 specularColor = vec4(light.specular, 1.0) * specularIntensity;

        float shadow = compute_shadows(light, vLightSpacePosition);
        vec4 result = ambientColor + (1.0 - shadow) * (diffuseColor + specularColor);

        return result;
    }

    void main() {
        vec4 color1 = compute_lighting(l);
        vec4 color2 = compute_lighting(d);
        vec4 color3 = compute_lighting(l2);

        vec4 total = color1 + color2 + color3;
        float z = gl_FragCoord.z / gl_FragCoord.w;
        float fog = clamp(exp(-fogIntensity * z * z), 0.01, 1.0);
        total = mix(fogColor, total, fog);

        outColor = total;
    }
);

class Test : public ic::Application {
    ic::Shader *shader, *depthShader, *skyboxShader;
    ic::Framebuffer *shadowMap;

    ic::Physics::PhysicsLevel2D *level;
    ic::Physics::RigidObject2D *ball, *wall;
    
    std::vector<ic::Mesh3D*> meshes;

    ic::Camera3D *camera;
    ic::Mesh3D *floorMesh, *ballMesh, *wallMesh;
    
    ic::Skybox *skybox;

    ic::Texture *floorTexture, *modelTexture, *wallTexture;
    
    float time;
    int shadowWidth, shadowHeight;

    ic::Spring2D cameraSpring;
    ic::FreeRoamCameraController3D *controller;

    public:
        bool init() override {
            displayName = "Scene Example";
            scaling = ic::WindowScaling::fullscreen;
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            
            shader = ic::ShaderLoader::get().load(vert, fragment);
            shader->use();
            shader->set_uniform_int("sampleTexture", 0);
            shader->set_uniform_int("shadowMap", 1);

            skyboxShader = ic::ShaderLoader::get().load(skyboxShaderVert, skyboxShaderFrag);


            depthShader = ic::ShaderLoader::get().load(depthShaderVert, depthShaderFrag);

            ic::TextureParameters params;
            params.usesMipmapping = true;
            
            floorTexture = ic::TextureLoader::get().load_png("resources/textures/sand.png", params);
            modelTexture = ic::TextureLoader::get().load_png("resources/textures/white.png", params);
            wallTexture = ic::TextureLoader::get().load_png("resources/textures/stone-bricks.png", params);
            
            shadowWidth = 1024 * 2;
            shadowHeight = 1024 * 2;
            shadowMap = new ic::Framebuffer(ic::TEXTURE_ATTACH_DEPTH, ic::TEXTURE_DEPTH, shadowWidth, shadowHeight);

            
            floorMesh = ic::GeometryGenerator::get().generate_parallelipiped_mesh(250.0f, 0.1f, 250.0f, 250.0f, 0.1f, 250.0f);
            floorMesh->set_transformation(ic::Mat4x4().set_translation<3>({0.0f, 0.0f, 0.0f}));

            ballMesh = ic::GeometryGenerator::get().generate_UV_sphere_mesh(0.5f, 12, 12);
            
            wallMesh = ic::GeometryGenerator::get().generate_parallelipiped_mesh(2.0f, 5.0f, 2.0f, 2.0f, 5.0f, 2.0f);
            
            skybox = new ic::Skybox({
                "resources/textures/skybox/right.png",
                "resources/textures/skybox/left.png",
                "resources/textures/skybox/top.png",
                "resources/textures/skybox/bottom.png",
                "resources/textures/skybox/front.png",
                "resources/textures/skybox/back.png",
            });

            cameraSpring.damping = 0.001f;
            cameraSpring.restLength = 2.0f;
            cameraSpring.stiffness = 3.0f;

            camera = new ic::Camera3D();
            camera->position = { 0.0f, 3.0f, -3.0f };
            
            controller = new ic::FreeRoamCameraController3D(camera, &inputHandler);
            controller->flying = true;

            level = new ic::Physics::PhysicsLevel2D();
            level->set_gravity(0.0f, -9.81f);
            level->simulationSteps = 5;

            ball = new ic::Physics::RigidObject2D();
            ball->collider = new ic::Physics::CircleCollider(0.5f);
            ball->set_position(0.0f, 3.0f);
            ball->apply_velocity(1.0f, 1.0f);
            ball->mass = 10.0f;

            level->add_object(ball);

            wall = new ic::Physics::RigidObject2D();
            wall->collider = new ic::Physics::RectangleCollider(2.0f, 5.0f);
            wall->set_position(100.0f, 5.0f);
            wall->dynamic = false;
            wall->restitution = 0.2f;

            level->add_object(wall);

            ic::Physics::RigidObject2D *floor = new ic::Physics::RigidObject2D();
            floor->collider = new ic::Physics::RectangleCollider(250.0f, 0.1f);
            floor->set_position(0.0f, 0.0f);
            floor->dynamic = false;

            floor->restitution = 0.4f;
            floor->staticFriction = 0.005f;
            floor->dynamicFriction = 0.5f;

            level->add_object(floor);

            time = 0.0f;
            
            ic::KeyboardController *keys = new ic::KeyboardController();
            keys->add_action([this](){ 
                float radius = 0.2f;

                ic::Physics::RigidObject2D *obj = new ic::Physics::RigidObject2D();
                obj->collider = new ic::Physics::CircleCollider(radius);
                obj->staticFriction = 0.001f;
                obj->dynamicFriction = 0.002f;
                obj->mass = 2.0f;
                //obj->gravity = { 0.0f, 9.81f };

                obj->set_position(-100.0f, rand() % 100 / 10.0f + 0.1f);
                obj->apply_velocity(25.0f + rand() % 1000 / 50.0f, -0.01f);
                level->add_object(obj);


                ic::Mesh3D *mesh = ic::GeometryGenerator::get().generate_UV_sphere_mesh(radius, 20, 20);
                meshes.push_back(mesh);
            }, KEY_SPACE);
            keys->add_key_up_action([this]() {
                ball->apply_velocity(-5.0f, 0.0f);
            }, KEY_LEFT);

            keys->add_key_up_action([this]() {
                ball->apply_velocity(5.0f, 0.0f);
            }, KEY_RIGHT);

            inputHandler.add_input(keys, "jumping");

            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            time += dt;
            

            level->update(dt);

            ic::Vec3f ballPos = { ball->transform->position.x(), ball->transform->position.y(), 0.0f };
            ic::Vec3f ballVel = { ball->velocity.x(), ball->velocity.y(), 0.0f };
            cameraSpring.update(ic::Vec2f({ camera->position.x(), camera->position.y() }), ball->transform->position, ic::Vec2f(), ball->velocity);

            camera->position = camera->position + ic::Vec3f({ cameraSpring.force1.x(), cameraSpring.force1.y(), 0.0f }) * dt;
            camera->lookingAt = ballPos;

            //controller->act(dt);


            camera->resize(IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            camera->update();

            ballMesh->set_transformation(ic::Mat4x4().set_translation<3>({ball->transform->position.x(), ball->transform->position.y(), 0.0f}));
            wallMesh->set_transformation(ic::Mat4x4().set_translation<3>({wall->transform->position.x(), wall->transform->position.y(), 0.0f}));
            for (int i = 2; i < meshes.size(); i++) {
                ic::Mesh3D *mesh = meshes[i];
                mesh->set_transformation(ic::Mat4x4().set_translation<3>({level->get_object(i)->transform->position.x(), level->get_object(i)->transform->position.y(), 0.0f}));
            }

            // Light projection
            ic::Mat4x4 lightProj = ic::Mat4x4().set_orthographic(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 200.0f);
            ic::Mat4x4 lightView = ic::Mat4x4().set_look_at(camera->position + ic::Vec3f({ -2.0f * 10.0f, 3.0f * 10.0f, -3.0f * 10.0f }), camera->position, { 0.0f, 1.0f, 0.0f });
            ic::Mat4x4 lightSpaceMatrix = lightProj * lightView;

            // First pass - render to the depth map
            states.set_viewport(shadowWidth, shadowHeight);
            states.disable_face_culling();
            shadowMap->use();
            clear_color(ic::Colors::black);
            depthShader->use();
            depthShader->set_uniform_mat4("lightSpaceMatrix", lightSpaceMatrix);
            render_scene(depthShader);
            shadowMap->unuse();
            


            // Second pass - render scene using shadow map
            states.set_viewport(IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            clear_color(ic::Colors::blue);
            render_skybox();

            states.enable_face_culling(ic::BACK, ic::CW);

            shader->use();
            camera->upload_to_shader(shader);
            shader->set_uniform_mat4("lightSpaceMatrix", lightSpaceMatrix);
            shader->set_uniform_vec3f("viewPosition", camera->position);

            shadowMap->use_texture(1);
            render_scene(shader);

            return true; 
        }

        void render_scene(ic::Shader *passShader) {
            modelTexture->use();
            ballMesh->draw(passShader);
            for (auto &mesh : meshes) {
                mesh->draw(passShader);
            }

            // Floor
            floorTexture->use();
            floorMesh->draw(passShader);

            // Wall
            wallTexture->use();
            wallMesh->draw(shader);
        }

        void render_skybox() {
            skyboxShader->use();
            camera->upload_to_shader(skyboxShader);
            skybox->draw(skyboxShader);
        }

        void dispose() override {
            shader->clear();
            depthShader->clear();

            floorMesh->dispose();
            skybox->dispose();
            ballMesh->dispose();
            wallMesh->dispose();

            floorTexture->dispose();
            modelTexture->use();
            wallTexture->use();

            shadowMap->dispose();
        }
};

int main(int argc, char *argv[]) {
    Test application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}