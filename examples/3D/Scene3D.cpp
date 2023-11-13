#include <Icosahedron/Core.h>

std::string fragment = IC_ADD_GLSL_DEFINITION(
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
    
    uniform vec3 diffuseColor = vec3(1.0, 1.0, 1.0);

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
        vec4 diffuseColor = vec4(diffuseColor * light.diffuse, 1.0) * diffuseIntensity;

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


/** A basic scene with a cube on a floor and a directional light. */
class Scene3D : public ic::Application {
    ic::Color meshColor, floorColor;
    ic::Shader *shader;

    ic::Camera3D *camera;
    ic::Mesh3D *mesh, *floorMesh;

    public:
        bool init() override {
            displayName = "Scene 3D example";
            scaling = ic::WindowScaling::fullscreen;
            
            return true;
        }
        
        bool load() override {
            states.enable_depth_testing(ic::LESS);
            
            shader = ic::ShaderLoader::get().load(shaders.meshShaderVertex3D, fragment);


            std::vector<float> positions = {
                -5.0f, 0.0f, -5.0f,
                5.0f, 0.0f, -5.0f,
                5.0f, 0.0f, 5.0f,
                -5.0f, 0.0f, 5.0f, 
            };

            std::vector<float> normals = {
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
            };

            mesh = ic::GeometryGenerator::get().generate_cube_mesh(0.5f);
            
            // A plane acting as a floor
            floorMesh = new ic::Mesh3D();
            floorMesh->add_attribute("position", 0, 3, positions);
            floorMesh->add_attribute("normal", 3, 3, normals);
            floorMesh->set_index_buffer({ 0, 1, 2, 0, 2, 3 });


            camera = new ic::Camera3D();
            camera->position = { -8.0f, 4.0f, 4.0f };
            camera->lookingAt = { 0.0f, 0.5f, 0.0f };

            meshColor = ic::Color().hexadecimal_to_RGB("BD5A4C");
            floorColor = ic::Color().hexadecimal_to_RGB("268B07");
            
            return true;
        }

        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        void window_size_changed(int w, int h) override {
            camera->resize(w, h);
        }

        bool update(float dt) override {
            camera->update();
            

            clear_color(ic::Colors::blue);
            
            shader->use();
            shader->set_uniform_vec3f("viewPosition", camera->position);
            camera->upload_to_shader(shader);

            states.enable_face_culling(ic::FRONT, ic::CCW);
            mesh->set_transformation(ic::Mat4x4().set_translation<3>({0.0f, 0.5f, 0.0f}));
            shader->set_uniform_color("diffuseColor", meshColor);
            mesh->draw(shader);

            // The calculated plane's normals might face away from the camera, 
            // so this function prevents the face from getting "culled" away from view
            states.disable_face_culling();
            shader->set_uniform_color("diffuseColor", floorColor);
            floorMesh->draw(shader);

            return true; 
        }

        void dispose() override {
            shader->clear();
            mesh->dispose();
            floorMesh->dispose();
        }
};


int main(int argc, char *argv[]) {
    Scene3D application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}