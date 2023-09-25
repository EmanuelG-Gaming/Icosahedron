#ifndef IC_EXAMPLE_TEXTURE_ARRAYS_H
#define IC_EXAMPLE_TEXTURE_ARRAYS_H

#include <Icosahedron/Core.h>


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
    ic::Shader *singleTextureShader, *textureArrayShader;
    
    ic::Camera3D *camera;
    ic::Mesh3D *mesh, *floorMesh;

    ic::Texture *floorTexture, *whiteTexture;
    ic::TextureArray *textureArray;

    ic::FreeRoamCameraController3D *controller;

    float time;
    public:
        bool init() override {
            displayName = "Example window";
            scaling = ic::WindowScaling::fullscreen;
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            states.enable_face_culling(ic::FRONT, ic::CCW);
            
            singleTextureShader = ic::ShaderLoader::get().load(shaders.meshShaderVertex3D, singleTextureFragment);
            textureArrayShader = ic::ShaderLoader::get().load(shaders.meshShaderVertex3D, textureArrayFragment);
            
            floorTexture = ic::TextureLoader::get().load_png("resources/textures/wood.png");
            whiteTexture = ic::TextureLoader::get().load_png("resources/textures/white.png");
            
            textureArray = new ic::TextureArray(64, 6);
            textureArray->add_texture("resources/textures/stone.png");
            textureArray->add_texture("resources/textures/stone-bricks.png");
            textureArray->add_texture("resources/textures/grass.png");
            textureArray->add_texture("resources/textures/dirt.png");
            textureArray->add_texture("resources/textures/wood.png");
            textureArray->add_texture("resources/textures/tile.png");

            mesh = new ic::Mesh3D(ic::GeometryGenerator::get().generate_cube(0.5f));
            mesh->jump_attribute();
            mesh->add_attribute("textureCoords", 2, ic::GeometryGenerator::get().generate_UV_parallelipiped());
            mesh->add_attribute("normal", 3, ic::GeometryGenerator::get().generate_normal_parallelipiped());
            mesh->set_index_buffer(ic::GeometryGenerator::get().generate_parallelipiped_indices());
            
            
            floorMesh = new ic::Mesh3D(ic::GeometryGenerator::get().generate_parallelipiped(5.0f * 5, 0.1f, 5.0f * 5));
            floorMesh->jump_attribute();
            floorMesh->add_attribute("textureCoords", 2, ic::GeometryGenerator::get().generate_UV_parallelipiped(10.0f * 5, 0.2f, 10.0f * 5));
            floorMesh->add_attribute("normal", 3, ic::GeometryGenerator::get().generate_normal_parallelipiped());
            floorMesh->set_index_buffer(ic::GeometryGenerator::get().generate_parallelipiped_indices());
            floorMesh->set_transformation(ic::Mat4x4().set_translation<3>({0.0f, 0.0f, 0.0f}));

            camera = new ic::Camera3D();
            camera->position = { -3.0f, 1.5f, 0.0f };
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
        }

        bool update(float dt) override {
            time += dt;

            controller->act(dt);
            camera->update();
            
            clear_color(ic::Colors::blue);
            
            singleTextureShader->use();
            singleTextureShader->set_uniform_vec3f("viewPosition", camera->position);
            camera->upload_to_shader(singleTextureShader);
            

            ic::Quaternion quat = ic::Quaternion().from_euler(0.0f, time, 0.0f);
            ic::Mat4x4 rotation = quat.to_rotation_matrix();
            ic::Mat4x4 translation = ic::Mat4x4().set_translation<3>({0.0f, 0.6f, 0.0f});
            mesh->set_transformation(translation * rotation);
            mesh->set_normal_transformation(rotation);
            

            whiteTexture->use();
            mesh->draw(singleTextureShader);

            floorTexture->use();
            floorMesh->draw(singleTextureShader);


            // Texture array rendering
            textureArrayShader->use();
            textureArray->use();

            textureArrayShader->set_uniform_vec3f("viewPosition", camera->position);
            camera->upload_to_shader(textureArrayShader);

            for (int i = 0; i < 6; i++) {
                ic::Quaternion q = ic::Quaternion().from_euler(0.0f, time, 0.0f);
                ic::Mat4x4 translation2 = ic::Mat4x4().set_translation<3>({5.0f, 0.6f, i * 2.0f - 3.0f});
                mesh->set_transformation(translation2);
                mesh->set_normal_transformation(ic::Mat4x4());

                textureArrayShader->set_uniform_int("textureIndex", i);


                mesh->draw(textureArrayShader);
            }
            
    
            return true; 
        }

        void dispose() override {
            singleTextureShader->clear();
            textureArrayShader->clear();

            mesh->dispose();
            floorMesh->dispose();
            floorTexture->dispose();
            whiteTexture->dispose();

            textureArray->dispose();
        }
};

#endif