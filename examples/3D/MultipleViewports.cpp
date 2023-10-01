#include <Icosahedron/Core.h>

std::string screenVertex = IC_ADD_GLSL_DEFINITION(
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec2 tCoords;
    layout (location = 2) in vec3 normal;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    
    out vec3 vPosition;
    out vec3 vNormal;
    out vec2 vTCoords;

    void main() {
        vec4 pos = (projection * view * model) * vec4(position, 1.0);

        vPosition = pos.xyz;
        vTCoords = vec2(tCoords.x, tCoords.y);
        
        gl_Position = pos;
    }
);

std::string screenFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;
    in vec2 vTCoords;

    uniform sampler2D screenTexture;

    out vec4 outColor;

    void main() {
        vec4 color = texture(screenTexture, vTCoords);
        outColor = color;
    }
);

std::string fragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;
    in vec2 vTCoords;
    in vec3 vNormal;
    struct PointLight {
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float constant;
        float linear;
        float quadratic;
    };
    PointLight l = PointLight(
        vec3(-2.0, 3.0, 1.0), 
        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.8, 0.7, 0.55), 
        
        1.0, 0.09, 0.032
    );
    
    uniform sampler2D sampleTexture;
    uniform vec3 viewPosition;
    out vec4 outColor;
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
        if (diffuseColor.a <= 0.1) diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
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
    
    void main() {
        vec4 color = compute_lighting(l);
        outColor = color;
    }
);

/** Renders the same scene to two different viewports (the first one is the player's view). */
class MultipleViewports : public ic::Application {
    ic::Shader *shader, *screenShader;
    ic::Framebuffer *framebuffer;
    
    ic::Camera3D *camera, *secondaryCamera;
    ic::Mesh3D *screenQuad;
    ic::Mesh3D *mesh;
    ic::Mesh3D *mainCameraMesh, *secondCameraMesh;

    ic::Texture *meshTexture, *mainCameraTexture;
    ic::FreeRoamCameraController3D *controller;

    float time;
    public:
        bool init() override {
            displayName = "Multiple Viewports Example";
            scaling = ic::WindowScaling::fullscreen;
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            
            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex3D, fragment);
            screenShader = ic::ShaderLoader::get().load(shaders.meshShaderVertex3D, fragment);
            
            meshTexture = ic::TextureLoader::get().load_png("resources/textures/wood.png");
            mainCameraTexture = ic::TextureLoader::get().load_png("resources/textures/white.png");
            
            framebuffer = new ic::Framebuffer(ic::TEXTURE_ATTACH_COLOR_0, ic::TEXTURE_RGBA, IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            

            mesh = ic::GeometryGenerator::get().generate_UV_sphere_mesh(0.5f, 14, 14);
            mainCameraMesh = ic::GeometryGenerator::get().generate_cube_mesh(0.5f);
            secondCameraMesh = ic::OBJLoader::get().get_mesh("resources/models/icosahedron.obj");

            screenQuad = new ic::Mesh3D();
            screenQuad->add_attribute("position", 0, 3, std::vector<float>({ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f }));
            screenQuad->add_attribute("textureCoords", 2, 2, ic::GeometryGenerator::get().generate_UV_rectangle());
            screenQuad->add_attribute("normal", 3, 3, std::vector<float>({ 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f }));
            screenQuad->set_index_buffer({ 0, 1, 2, 0, 2, 3 });

            camera = new ic::Camera3D();
            camera->position = { -3.0f, 1.5f, 0.0f };
            
            secondaryCamera = new ic::Camera3D();
            secondaryCamera->position = { 3.0f, 5.0f, 3.0f };
            secondaryCamera->lookingAt = { 0.0f, 0.0f, 0.0f };

            controller = new ic::FreeRoamCameraController3D(camera, &inputHandler);
            controller->flying = true;
            time = 0.0f;
            
            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        void window_size_changed(int w, int h) override {
            camera->resize(w, h);
            secondaryCamera->resize(w, h);
            framebuffer->resize(w, h);
        }

        bool update(float dt) override {
            time += dt;

            controller->act(dt);
            camera->update();
            secondaryCamera->update();

            // First pass - scene drawing
            framebuffer->use();
            
            clear_color(ic::Colors::blue);
            
            render_scene(secondaryCamera);

            framebuffer->unuse();


            clear_color(ic::Colors::blue);
            
            // Second pass - drawing via framebuffer
            states.disable_face_culling();
            screenShader->use();
            screenShader->set_uniform_vec3f("viewPosition", camera->position);
            camera->upload_to_shader(screenShader);

            ic::Mat4x4 translation2 = ic::Mat4x4().set_translation<3>({ -1.0f, 0.0f, 2.5f });
            ic::Mat4x4 scaling = ic::Mat4x4().set_scaling<3>({ 1.0f * (IC_WINDOW_WIDTH / (float) IC_WINDOW_HEIGHT), 1.0f, 1.0f });
            screenQuad->set_transformation(translation2 * scaling);

            framebuffer->use_texture();
            screenQuad->draw(screenShader);

            render_scene(camera);

            return true;
        }

        void render_scene(ic::Camera3D *cam) {
            states.enable_face_culling(ic::FRONT, ic::CCW);
            
            shader->use();
            shader->set_uniform_vec3f("viewPosition", cam->position);
            cam->upload_to_shader(shader);
            
            ic::Quaternion quat = ic::Quaternion().from_euler(0.0f, time, 0.0f);
            ic::Mat4x4 rotation = quat.to_rotation_matrix();
            ic::Mat4x4 translation = ic::Mat4x4().set_translation<3>({0.0f, 0.6f, 0.0f});
            mesh->set_transformation(translation * rotation);
            mesh->set_normal_transformation(rotation);
            

            meshTexture->use();
            mesh->draw(shader);



            ic::Mat4x4 translation2 = ic::Mat4x4().set_translation<3>(camera->position);
            mainCameraMesh->set_transformation(translation2);
            
            ic::Mat4x4 translation3 = ic::Mat4x4().set_translation<3>(secondaryCamera->position);
            secondCameraMesh->set_transformation(translation3);

            mainCameraTexture->use();
            mainCameraMesh->draw(shader);
            secondCameraMesh->draw(shader);
        }

        void dispose() override {
            shader->clear();
            mesh->dispose();
            meshTexture->dispose();
            framebuffer->dispose();
        }
};

int main(int argc, char *argv[]) {
    MultipleViewports application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}