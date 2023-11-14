#include <Icosahedron/Application.h>

#include <Icosahedron/math/geom/Quaternion.h>

#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Texture.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/3d/controllers/OrbitalCameraController3D.h>
#include <Icosahedron/scene/3d/Camera3D.h>
#include <Icosahedron/scene/3d/Mesh3D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>
#include <Icosahedron/assets/loaders/TextureLoader.h>
#include <Icosahedron/assets/loaders/OBJLoader.h>


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
        vec3(3.0, 3.0, 3.0), 
        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.8, 0.7, 0.55), 
        
        1.0, 0.09, 0.032
    );
    
    struct Material {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        float shininess;
    };

    uniform Material material;
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
        vec4 ambientColor = vec4(light.ambient * material.ambient, 1.0) * ambientIntensity;

        // Diffuse reflection
        float diffuseIntensity = clamp(dotProduct, 0.0, 1.0) * attenuation;
        vec4 diffuseColor = vec4(light.diffuse * (material.diffuse * diffuseIntensity), 1.0);

        // Specular reflection
        // Blinn-Phong reflection
        vec3 reflectDirection = normalize(lightDirection + viewDirection);
        float specularIntensity = pow(max(dot(vNormal, reflectDirection), 0.0), material.shininess / 4.0) * attenuation;
        
        // Basic Phong reflection
        //vec3 reflectDirection = reflect(-lightDirection, normal); 
        //float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess * 4.0) * attenuation;
        
        vec4 specularColor = vec4(light.specular * (material.specular * specularIntensity), 1.0);

        vec4 result = ambientColor + diffuseColor + specularColor;
        return result;
    }
    
    void main() {
        vec4 color = compute_lighting(l);
        outColor = color;
    }
);


/** A model viewer. Press up/down keys to increase or decrease the view radius.
 *  Use the Q key to switch between perspective and orthographic projections, and
 *  the R key to make the viewed object rotate.
*/
class ModelViewerDemo : public ic::Application {
    ic::Shader *shader;
    ic::Camera3D *camera;
    ic::Mesh3D *mesh;
    ic::OrbitalCameraController3D *controller;

    ic::OBJMaterialInfo material;

    float scale;
    float delta;
    float time;
    bool perspective;
    bool rotating;

    public:
        bool init() override {
            displayName = "Model Viewer Demo";
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            states.enable_face_culling(ic::FRONT, ic::CCW);
            
            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex3D, fragment);
            
            mesh = ic::OBJLoader::get().get_mesh("resources/models/boat.obj");
            mesh->set_transformation(ic::Mat4x4().set_translation<3>({0.0f, 0.0f, 0.0f}));

            material = ic::OBJLoader::get().get_materials("resources/models/icosahedron.mtl")["Material.001"];

            scale = 3.0f;
            perspective = true;
            rotating = false;
            delta = 0.0f;
            time = 0.0f;

            camera = new ic::Camera3D(perspective);
            camera->position = { -3.0f, 1.5f, 0.0f };
            controller = new ic::OrbitalCameraController3D(camera, &ic::InputHandler::get());
            
            // Inputs
            auto scaling = new ic::KeyboardController();
            scaling->add_action([this](){ scale -= 5 * delta; }, KEY_UP);
            scaling->add_action([this](){ scale += 5 * delta; }, KEY_DOWN);

            ic::InputHandler::get().add_input(scaling, "orbitScaling");

            auto toggle = new ic::KeyboardController();
            toggle->add_key_up_action([this](){ perspective = !perspective; }, KEY_Q);
            toggle->add_key_up_action([this](){ rotating = !rotating; }, KEY_R);
            
            ic::InputHandler::get().add_input(toggle, "toggle");

            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }
    
        bool update(float dt) override {
            delta = dt;
            if (rotating) time += delta;
            if (scale < 0.2f) scale = 0.2f;

            camera->settings.perspective = perspective;
            controller->radius = scale;

            float div = perspective ? 1.0f : (1 / scale) * 800.0f;
            controller->act(dt);
            camera->resize(IC_WINDOW_WIDTH / div, IC_WINDOW_HEIGHT / div);
            camera->update();
            
            clear_color(ic::Colors::blue);
            shader->use();
            camera->upload_to_shader(shader);

            shader->set_uniform_vec3f("viewPosition", camera->position);
            shader->set_uniform_vec3f("material.ambient", material.ambient);
            shader->set_uniform_vec3f("material.diffuse", material.diffuse);
            shader->set_uniform_vec3f("material.specular", material.specular);
            shader->set_uniform_float("material.shininess", material.shininess);
            
            
            //for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) {
            //    ic::Quaternion quat = ic::Quaternion().from_euler(0.0f, time, 0.0f);
            //    ic::Mat4x4 rotation = quat.to_rotation_matrix();
            //    ic::Mat4x4 translation = ic::Mat4x4().set_translation<3>({i * 2.0f, 0.0f, j * 2.0f});
            //    
            //    mesh->set_transformation(translation * rotation);
            //    mesh->set_normal_transformation(rotation);
            //    mesh->draw(shader);
            //}
            

            ic::Quaternion quat = ic::Quaternion().from_euler(0.0f, time, 0.0f);
            ic::Mat4x4 rotation = quat.to_rotation_matrix();
               
            mesh->set_transformation(rotation);
            mesh->set_normal_transformation(rotation);
            mesh->draw(shader);

            return true; 
        }

        void dispose() override {
            shader->clear();
            mesh->dispose();;
        }
};


int main(int argc, char *argv[]) {
    ModelViewerDemo application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}