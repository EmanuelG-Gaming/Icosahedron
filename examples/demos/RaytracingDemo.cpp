#include <Icosahedron/Application.h>
#include <Icosahedron/util/GeometryGenerator.h>

#include <Icosahedron/graphics/gl/Framebuffer.h>
#include <Icosahedron/graphics/gl/Shader.h>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/graphics/Colors.h>

#include <Icosahedron/scene/2d/Mesh2D.h>
#include <Icosahedron/scene/3d/Camera3D.h>
#include <Icosahedron/scene/3d/controllers/FreeRoamCameraController3D.h>

#include <Icosahedron/assets/loaders/ShaderLoader.h>


const std::size_t RAYTRACING_WIDTH = 640;
const std::size_t RAYTRACING_HEIGHT = 480;

std::string rayVertex = IC_ADD_GLSL_DEFINITION(
    layout (location = 0) in vec2 position;
    layout (location = 1) in vec3 color;
    layout (location = 2) in vec2 tCoords;

    out vec2 vPosition;
    out vec2 vTCoords;

    void main() {
        vPosition = position;
        vTCoords = tCoords;
        
        gl_Position = vec4(vPosition, 0.0, 1.0);
    }
);

std::string rayFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;

    in vec2 vPosition;
    in vec2 vTCoords;

    struct RayResult {
        vec3 intersection;
        vec3 normal;
        float t;
        bool hit;
    };

    struct Ray {
        vec3 origin;
        vec3 direction;
    };
    
    
    struct Sphere {
        vec3 center;
        float radius;
        vec3 diffuse;
    };


    const int MAX_SPHERE_COUNT = 3;
    uniform Sphere spheres[MAX_SPHERE_COUNT];
    uniform vec3 lightPosition = vec3(1.0, 1.0, 1.0);
    uniform mat4 projection;


    RayResult ray_trace_sphere(Ray ray, Sphere sphere) {
        vec3 offset = ray.origin - sphere.center;

        // Form a 2nd order polynominal ax^2 + bx + c
        float a = dot(ray.direction, ray.direction);
        float b = 2.0 * dot(ray.direction, offset);
        float c = dot(offset, offset) - sphere.radius * sphere.radius;

        float discriminant = b*b - 4.0*a*c;
        
        RayResult result;
        if (discriminant > 0.0) {
            float alpha = (-b + sqrt(discriminant)) / (2.0 * a);
            float alpha2 = -b / a - alpha;
            if (abs(alpha2) < abs(alpha)) alpha = alpha2;
    
            result.intersection = ray.origin + ray.direction * alpha;
            result.normal = normalize(result.intersection - sphere.center);
            result.t = alpha;
            result.hit = true;

            return result;
        } else if (discriminant == 0.0) {
            // Tries to fix background noise
        
            float alpha = -b / (2.0 * a);
            float alpha2 = -b / a - alpha;
            if (abs(alpha2) < abs(alpha)) alpha = alpha2;
    
            result.intersection = ray.origin + ray.direction * alpha;
            result.normal = normalize(result.intersection - sphere.center);
            result.t = alpha;
            result.hit = true;

            return result;
        }

        result.hit = false;
        return result;
    }

    vec4 sphere_shading(RayResult intersection, Sphere sphere) {
        vec3 lightDir = normalize(lightPosition - intersection.intersection);
        vec3 normal = normalize(intersection.normal);
        float intensity = clamp(dot(lightDir, normal), 0.0, 1.0);

        vec3 diffuse = sphere.diffuse * intensity;
        vec4 color = vec4(diffuse, 1.0);

        return color;
    }


    vec4 ray_color(Ray ray, inout RayResult minIntersection) {
        float minimumT = 90000.0;
        vec4 color = vec4(0.0, 0.0, 1.0, 1.0);
        
        for (int i = 0; i < MAX_SPHERE_COUNT; i++) {
            Sphere base = spheres[i];

            RayResult rayRes = ray_trace_sphere(ray, base);
            if (rayRes.hit) {
                float intersectionT = rayRes.t;

                // Keep track of the minimum distance between the spheres and the ray position
                if (intersectionT < minimumT) {
                    minimumT = intersectionT;
                    color = sphere_shading(rayRes, base);
                    minIntersection = rayRes;
                }
            }
        }

        return color;    
    }

    vec4 reflect_ray(Ray ray, int iterations) {
        vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
        Ray currentRay = ray;

        for (int i = 0; i < iterations; i++) {
            RayResult intersection1;
            vec4 color1 = ray_color(currentRay, intersection1);
    
            vec3 reflectPosition = intersection1.intersection;
            vec3 reflectDirection = reflect(-ray.direction, intersection1.normal);
    
            RayResult intersection2;
            Ray reflected = Ray(reflectPosition, reflectDirection);
    
            vec4 color2 = ray_color(reflected, intersection2);
            sum += color1 + color2;
            currentRay = reflected;
        }
        sum /= (iterations * 2);

        return sum;
    }

    vec4 shadowing(Ray ray) {
        float minimumT = 90000.0;
        RayResult minRes;
        Sphere baseSphere;

        for (int i = 0; i < MAX_SPHERE_COUNT; i++) {
            Sphere base = spheres[i];

            RayResult rayRes = ray_trace_sphere(ray, base);
            if (!rayRes.hit) continue;
            float intersectionT = rayRes.t;

            // Keep track of the minimum distance between the spheres and the ray position
            if (intersectionT < minimumT) {
                minimumT = intersectionT;
                baseSphere = base;
                minRes = rayRes;
            }
        }

        vec3 biased = minRes.intersection + minRes.normal * 0.001;
        Ray shadowRay = Ray(biased, normalize(biased - lightPosition));
        for (int j = 0; j < MAX_SPHERE_COUNT; j++) {
            Sphere base2 = spheres[j];
            if (baseSphere == base2) continue;

            RayResult shadowRes = ray_trace_sphere(shadowRay, base2);
            if (shadowRes.hit) {
                return vec4(0.2, 0.2, 0.2, 1.0);
            }
        }

        return vec4(1.0, 1.0, 1.0, 1.0);
    }

    out vec4 outColor;

    void main() {
        vec2 centered = vTCoords * 2.0 - 1.0;

        mat4 inverseProj = inverse(projection);
        vec3 pos = vec3(centered.xy, 0.0);
        vec3 dir = normalize((inverseProj * vec4(pos, -1.0))).xyz;
        Ray ray = Ray(vec3(0.0, 0.0, 0.0), dir);

        vec4 average = reflect_ray(ray, 2);
        vec4 shadow = shadowing(ray);

        outColor = average * shadow;
    }
);


std::string screenFragment = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec2 vPosition;
    in vec2 vTCoords;

    uniform sampler2D screenTexture;

    out vec4 outColor;

    void main() {
        vec4 color = texture(screenTexture, vTCoords);
        outColor = color;
    }
);

/* A very simple ray tracer. */
class RaytracingDemo : public ic::Application {
    ic::Shader screenShader, rayShader;
    ic::Framebuffer framebuffer;
    
    ic::Mesh2D screenQuad;
    ic::Camera3D camera;
    float time;

    public:
        bool init() override {
            displayName = "Raytracer Demo";
            
            return true;
        }
        
        bool load() override {
            rayShader = ic::ShaderLoader::load(rayVertex, rayFragment);
            screenShader = ic::ShaderLoader::load(ic::Shaders::meshShaderVertex2D, screenFragment);
            
            // Initialize these values directly
            rayShader.use();
            rayShader.set_uniform_vec3f("spheres[0].center", { 0.0f, 0.2f, 1.5f });
            rayShader.set_uniform_color("spheres[0].diffuse", ic::Colors::red);
            rayShader.set_uniform_float("spheres[0].radius", 0.05f);

            rayShader.set_uniform_vec3f("spheres[1].center", { 0.5f, 0.0f, 1.5f });
            rayShader.set_uniform_color("spheres[1].diffuse", ic::Colors::green);
            rayShader.set_uniform_float("spheres[1].radius", 0.35f);

            rayShader.set_uniform_vec3f("spheres[2].center", { -0.4f, -0.1f, 1.5f });
            rayShader.set_uniform_color("spheres[2].diffuse", ic::Colors::cyan);
            rayShader.set_uniform_float("spheres[2].radius", 0.5f);

            framebuffer = ic::Framebuffer(ic::TEXTURE_ATTACH_COLOR_0, ic::TEXTURE_RGBA, RAYTRACING_WIDTH, RAYTRACING_HEIGHT);

            screenQuad = ic::GeometryGenerator::generate_rectangle_mesh(1.0f, 1.0f);
            
            camera = ic::Camera3D();

            time = 0.0f;

            return true;
        }


        bool handle_event(ic::Event event, float dt) override { 
            return true;
        }

        bool update(float dt) override {
            time += dt;
            camera.update();

            clear_color(ic::Colors::blue);
            
            // First pass - getting raytraced information
            framebuffer.use();
            rayShader.use();
            rayShader.set_uniform_vec3f("lightPosition", { 0.5f, 0.1f, 0.0f });
            camera.upload_to_shader(rayShader);
            rayShader.set_uniform_vec3f("spheres[0].center", { ic::Mathf::sinf(time * 0.5f) * 0.3f, 0.0f, 0.5f });
            
            screenQuad.draw(rayShader);
            framebuffer.unuse();

            // Second pass - drawing the screen-covering quadrilateral
            screenShader.use();
            framebuffer.use_texture();
            screenQuad.draw(screenShader);

            return true; 
        }

        void dispose() override {
            screenShader.clear();
            rayShader.clear();

            framebuffer.dispose();
            screenQuad.dispose();
        }
};

int main() {
    RaytracingDemo application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}