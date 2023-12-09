#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/math/geom/Quaternion.h>

#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/gl/Framebuffer.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/graphics/Image.h>
#include <Icosahedron/graphics/ImageIO.h>

#include <Icosahedron/scene/3d/controllers/FreeRoamCameraController3D.h>
#include <Icosahedron/scene/3d/Camera3D.h>
#include <Icosahedron/scene/3d/Mesh3D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>
#include <Icosahedron/assets/loaders/OBJLoader.h>


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

    PointLight l = PointLight(
        vec3(-2.0, 3.0, -3.0), 
        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.8, 0.7, 0.55), 
        
        1.0, 0.09, 0.032
    );
    
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
        if (diffuseColor.a <= 0.1) discard; //diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
        else diffuseColor *= vec4(light.diffuse, 1.0) * diffuseIntensity;

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), (0.1 * 128.0) * 4.0) * attenuation;
        
        vec4 specularColor = vec4(light.specular, 1.0) * specularIntensity;

        float shadow = compute_shadows(light, vLightSpacePosition);
        vec4 result = ambientColor + (1.0 - shadow) * (diffuseColor + specularColor);

        return result;
    }
    
    void main() {
        vec4 color = compute_lighting(l);
        outColor = color;
    }
);


class SceneWithShadows : public ic::Application {
    ic::Shader shader, depthShader;
    ic::Framebuffer shadowMap;

    ic::Camera3D camera;
    ic::Mesh3D mesh, floorMesh;
    ic::Texture floorTexture, whiteTexture;
    ic::FreeRoamCameraController3D controller;

    float time;
    int shadowWidth, shadowHeight;

    public:
        bool init() override {
            displayName = "Scene Example";
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            
            shader = ic::ShaderLoader::get().load(vert, fragment);
            shader.use();
            shader.set_uniform_int("sampleTexture", 0);
            shader.set_uniform_int("shadowMap", 1);

            depthShader = ic::ShaderLoader::get().load(depthShaderVert, depthShaderFrag);

            ic::TextureParameters floorParams;
            floorParams.usesMipmapping = true;
            
            floorTexture = ic::TextureLoader::get().load_png("resources/textures/stone-bricks.png", floorParams);
            whiteTexture = ic::TextureLoader::get().load_png("resources/textures/white.png");

            shadowWidth = 1024;
            shadowHeight = 1024;
            shadowMap = ic::Framebuffer(ic::TEXTURE_ATTACH_DEPTH, ic::TEXTURE_DEPTH, shadowWidth, shadowHeight);

            mesh = ic::OBJLoader::get().load("resources/models/vector.obj");
            
            floorMesh = ic::GeometryGenerator::get().generate_parallelipiped_mesh(25.0f, 0.1f, 25.0f, 25.0f, 0.1f, 25.0f);
            floorMesh.set_transformation(ic::Mat4x4().set_translation<3>({0.0f, 0.0f, 0.0f}));

            camera = ic::Camera3D();
            camera.position = { -3.0f, 1.5f, 0.0f };

            controller = ic::FreeRoamCameraController3D(&camera);
            controller.flying = true;
            
            time = 0.0f;

            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            time += dt;
            
            controller.act(dt);
            camera.resize(IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            camera.update();


            // First pass - render to the depth map
            states.set_viewport(shadowWidth, shadowHeight);
            states.disable_face_culling();

            shadowMap.use();
            clear_color(ic::Colors::black);

            depthShader.use();

            // Render to directional shadow map
            ic::Mat4x4 lightProj = ic::Mat4x4().set_orthographic(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 30.0f);
            ic::Mat4x4 lightView = ic::Mat4x4().set_look_at({ -2.0f * 2, 3.0f * 2, -3.0f * 2 }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
            ic::Mat4x4 lightSpaceMatrix = lightProj * lightView;

            depthShader.set_uniform_mat4("lightSpaceMatrix", lightSpaceMatrix);
            render_scene(depthShader);

            shadowMap.unuse();
            
            // Second pass - render scene using shadow map
            states.set_viewport(IC_WINDOW_WIDTH, IC_WINDOW_HEIGHT);
            states.enable_face_culling(ic::BACK, ic::CW);

            clear_color(ic::Colors::blue);

            shader.use();
            camera.upload_to_shader(shader);
            shader.set_uniform_vec3f("viewPosition", camera.position);
            shader.set_uniform_mat4("lightSpaceMatrix", lightSpaceMatrix);

            shadowMap.use_texture(1);
            render_scene(shader);

            return true; 
        }

        void render_scene(ic::Shader &passShader) {
            // Mesh
            ic::Quaternion quat = ic::Quaternion().from_euler(0.0f, time, 0.0f);
            ic::Mat4x4 rotation = quat.to_rotation_matrix();
            ic::Mat4x4 translation = ic::Mat4x4().set_translation<3>({0.0f, 0.6f, 0.0f});
            
            mesh.set_transformation(translation * rotation);
            mesh.set_normal_transformation(rotation);
            whiteTexture.use();
            mesh.draw(passShader);

            // Floor
            floorTexture.use();
            floorMesh.draw(passShader);
        }

        void dispose() override {
            shader.clear();
            depthShader.clear();

            mesh.dispose();
            floorMesh.dispose();

            floorTexture.dispose();
            whiteTexture.dispose();

            shadowMap.dispose();
        }
};

int main(int argc, char *argv[]) {
    SceneWithShadows application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}