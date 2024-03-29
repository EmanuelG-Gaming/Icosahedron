#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/math/geom/Quaternion.h>

#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/gl/TextureArray.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/3d/controllers/FreeRoamCameraController3D.h>
#include <Icosahedron/scene/3d/Camera3D.h>
#include <Icosahedron/scene/3d/Mesh3D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>



std::string singleTextureFragment = IC_ADD_GLSL_DEFINITION(
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
        vec3(-2.0, 3.0, -3.0), 
        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.8, 0.7, 0.55), 
        
        1.0, 0.09, 0.032
    );
    PointLight l2 = PointLight(
        vec3(10.0, 3.0, 3.5), 

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
        vec4 color1 = compute_lighting(l);
        vec4 color2 = compute_lighting(l2);

        outColor = color1 + color2;
    }
);

std::string textureArrayFragment = IC_ADD_GLSL_DEFINITION(
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
        vec3(-2.0, 3.0, -3.0), 

        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.8, 0.7, 0.55), 
        
        1.0, 0.09, 0.032
    );
    PointLight l2 = PointLight(
        vec3(10.0, 3.0, 3.5), 

        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.8, 0.7, 0.55), 
        
        1.0, 0.09, 0.032
    );

    uniform sampler2DArray sampleTexture;
    uniform vec3 viewPosition;
    uniform int textureIndex = 0;

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
        vec4 diffuseColor = texture(sampleTexture, vec3(vTCoords, float(textureIndex)));
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
        vec4 color1 = compute_lighting(l);
        vec4 color2 = compute_lighting(l2);

        outColor = color1 + color2;
    }
);

/** Texture arrays can be used to efficiently sample from successive texture layers,
 *  making them an alternative to texture atlases.
*/
class TextureArrays : public ic::Application {
    ic::Shader singleTextureShader, textureArrayShader;
    
    ic::Camera3D camera;
    ic::Mesh3D mesh, floorMesh;

    ic::Texture floorTexture, whiteTexture;
    ic::TextureArray textureArray;

    ic::FreeRoamCameraController3D controller;

    float time;
    public:
        bool init() override {
            displayName = "Texture Array Example";
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            ic::GLStateHandler::enable_depth_testing(ic::LESS);
            ic::GLStateHandler::enable_face_culling(ic::FRONT, ic::CCW);
            
            singleTextureShader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex3D, singleTextureFragment);
            textureArrayShader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex3D, textureArrayFragment);
            
            floorTexture = ic::TextureLoader::load_png("resources/textures/wood.png");
            whiteTexture = ic::TextureLoader::load_png("resources/textures/white.png");
            
            textureArray = ic::TextureArray(64, 6);
            textureArray.add_texture("resources/textures/stone.png");
            textureArray.add_texture("resources/textures/stone-bricks.png");
            textureArray.add_texture("resources/textures/grass.png");
            textureArray.add_texture("resources/textures/dirt.png");
            textureArray.add_texture("resources/textures/wood.png");
            textureArray.add_texture("resources/textures/tile.png");

            mesh = ic::GeometryGenerator::generate_cube_mesh(0.5f);

            floorMesh = ic::GeometryGenerator::generate_parallelipiped_mesh(25.0f, 0.1f, 25.0f, 25.0f, 0.1f, 25.0f);
            floorMesh.set_transformation(ic::Mat4x4().set_translation<3>({0.0f, 0.0f, 0.0f}));

            camera = new ic::Camera3D();
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
            camera.update();
            
            clear_color(ic::Colors::blue);
            
            singleTextureShader.use();
            singleTextureShader.set_uniform_vec3f("viewPosition", camera.position);
            camera.upload_to_shader(singleTextureShader);
            

            ic::Quaternion quat = ic::Quaternion().from_euler(0.0f, time, 0.0f);
            ic::Mat4x4 rotation = quat.to_rotation_matrix();
            ic::Mat4x4 translation = ic::Mat4x4().set_translation<3>({0.0f, 0.6f, 0.0f});
            mesh.set_transformation(translation * rotation);
            mesh.set_normal_transformation(rotation);
            

            whiteTexture.use();
            mesh.draw(singleTextureShader);

            floorTexture.use();
            floorMesh.draw(singleTextureShader);


            // Texture array rendering
            textureArrayShader.use();
            textureArray.use();

            textureArrayShader.set_uniform_vec3f("viewPosition", camera.position);
            camera.upload_to_shader(textureArrayShader);

            for (int i = 0; i < 6; i++) {
                ic::Quaternion q = ic::Quaternion().from_euler(0.0f, time, 0.0f);
                ic::Mat4x4 translation2 = ic::Mat4x4().set_translation<3>({5.0f, 0.6f, i * 2.0f - 3.0f});
                mesh.set_transformation(translation2);
                mesh.set_normal_transformation(ic::Mat4x4());

                textureArrayShader.set_uniform_int("textureIndex", i);
                mesh.draw(textureArrayShader);
            }
            
    
            return true; 
        }

        void dispose() override {
            singleTextureShader.clear();
            textureArrayShader.clear();

            mesh.dispose();
            floorMesh.dispose();
            floorTexture.dispose();
            whiteTexture.dispose();

            textureArray.dispose();
        }
};

int main() {
    TextureArrays application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}