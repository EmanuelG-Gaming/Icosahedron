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
#include <IcosahedronDebug/ConsoleOutput.h>


const std::size_t RAYMARCHING_WIDTH = 640;
const std::size_t RAYMARCHING_HEIGHT = 480;

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
    precision highp float;

    in vec2 vPosition;
    in vec2 vTCoords;
    
    struct Sphere {
        vec3 center;
        float radius;
        vec3 diffuse;
    };

    struct Box {
        vec3 center;
        vec3 sizes;
    };

    struct RaymarchResult {
        float distanceTravelled;
        int steps;
        bool hitObject;
    };

    struct ReflectionResult {
        vec3 color;
        vec3 nextHitPoint;
        vec3 nextDirection;
    };

    struct DirectionalLight {
        vec3 direction;
    };
    
    struct PointLight {
        vec3 position;
    };

    uniform DirectionalLight sun = DirectionalLight(vec3(1.0, -1.0, -1.0));
    uniform PointLight light1 = PointLight(vec3(4.0, 3.0, -1.0));
    uniform PointLight light2 = PointLight(vec3(-2.0, 3.0, 3.0));

    const int MAX_SPHERE_COUNT = 3;
    const int MAX_BOX_COUNT = 2;

    const int RAYMARCH_STEPS = 128;
    const float MAX_RAYMARCH_DISTANCE = 100;
    const float SURFACE_DISTANCE_THRESHOLD = 0.001;

    const int REFLECTION_ITERATIONS = 3;

    const int SHADOW_STEPS = 16;
    const float SHADOW_THRESHOLD = 0.001;

    const int AMBIENT_OCCLUSION_STEPS = 8;
    const float AMBIENT_OCCLUSION_STEP_SIZE = 0.04;

        
    uniform Sphere spheres[MAX_SPHERE_COUNT];
    uniform Box boxes[MAX_BOX_COUNT];
    uniform mat4 view;
    
    uniform mat4 projection;
    uniform vec3 cameraPosition;
    
    out vec4 outColor;


    float get_sphere_distance(Sphere sphere, vec3 point) {
        return length(point - sphere.center) - sphere.radius;
    }

    float get_box_distance(Box box, vec3 point) {
        vec3 difference = point - box.center;

        float cx = clamp(difference.x, -box.sizes.x, box.sizes.x);
        float cy = clamp(difference.y, -box.sizes.y, box.sizes.y);
        float cz = clamp(difference.z, -box.sizes.z, box.sizes.z);

        return length(difference) - length(vec3(cx, cy, cz));
    }


    float get_global_distance(vec3 point) {
        float distanceToSphere = 100000.0;
        float distanceToBox = 100000.0;

        for (int i = 0; i < MAX_SPHERE_COUNT; i++) {
            float distance = get_sphere_distance(spheres[i], point);
            if (distance < distanceToSphere) {
                distanceToSphere = distance;
            }
        }

        for (int i = 0; i < MAX_BOX_COUNT; i++) {
            float distance = get_box_distance(boxes[i], point);
            if (distance < distanceToBox) {
                distanceToBox = distance;
            }
        }


        float distanceToPlane = point.y + 1.0;
        float result = min(distanceToSphere, distanceToPlane);
        result = min(result, distanceToBox);

        return result;
    }

    RaymarchResult ray_march(vec3 origin, vec3 direction) {
        RaymarchResult result;
        result.distanceTravelled = 0.0;
        result.steps = 0;
        result.hitObject = false;

        for (int i = 0; i < RAYMARCH_STEPS; i++) {
            vec3 supportPoint = origin + direction * result.distanceTravelled;
            float distance = get_global_distance(supportPoint);

            result.distanceTravelled += distance;
            result.steps++;
            if (result.distanceTravelled > MAX_RAYMARCH_DISTANCE) {
                break;
            }

            if (distance < SURFACE_DISTANCE_THRESHOLD) {
                result.hitObject = true;
                break;
            }
        }

        return result;
    }

    vec3 get_normal(vec3 point) {
        float distance = get_global_distance(point);

        vec3 step = vec3(SURFACE_DISTANCE_THRESHOLD, 0.0, 0.0);

        float gradientX = get_global_distance(point + step.xyy) - get_global_distance(point - step.xyy);
        float gradientY = get_global_distance(point + step.yxy) - get_global_distance(point - step.yxy);
        float gradientZ = get_global_distance(point + step.yyx) - get_global_distance(point - step.yyx);

        vec3 normal = vec3(gradientX, gradientY, gradientZ);

        return normalize(normal);
    }

    float shadow(vec3 point, vec3 normal, vec3 lightDirection) {
        vec3 bias = normal * SURFACE_DISTANCE_THRESHOLD;
        vec3 origin = point + bias;
        vec3 direction = normalize(lightDirection);
        
        // Soft shadows
        float result = 1.0;
        float t = SURFACE_DISTANCE_THRESHOLD;
        
        for (int i = 0; t < MAX_RAYMARCH_DISTANCE && i < RAYMARCH_STEPS; i++) {
            float d = get_global_distance(origin + direction * t);
            if (d < SHADOW_THRESHOLD) {
                return 0.0;
            }

            result = min(result, SHADOW_STEPS * d / t);
            t += d;
        }
        
        return result;
    }


    vec3 get_sky_color(vec3 direction) {
        vec3 sunPosition = -sun.direction;
        vec3 moonPosition = -sunPosition;

        float t = clamp(sunPosition.y, 0.0, 1.0);
        vec3 nightSkyColor = vec3(0.0, 0.0, 0.05);
        vec3 skyColor = mix(nightSkyColor, vec3(0.0, 0.0, 1.0), t);

        vec3 sunColor = mix(nightSkyColor, vec3(1.0, 1.0, 0.0), t);
        vec3 moonColor = mix(vec3(0.3, 0.3, 0.3), vec3(0.0, 0.0, 0.0), t);
        
        float dotProd = clamp(dot(normalize(sunPosition), normalize(direction)), 0.0, 1.0);
        float moonDot = clamp(dot(normalize(moonPosition), normalize(direction)), 0.0, 1.0);
        float rayleigh = (3.0 / (8.0 * 3.14) * (1.0 + dotProd * dotProd)) / (abs(sunPosition.y) + 1.0) * 5.0;
        
        float threshold = moonDot > 0.95 ? 1.0 : 0.0;

        // Interpolate between the sky color and sun color
        return mix(skyColor, sunColor, rayleigh) + mix(vec3(0.0, 0.0, 0.0), moonColor, threshold);
    }

    vec3 lighting(PointLight light, vec3 rayDirection, vec3 point, vec3 normal) {
        vec3 fullDirection = light.position - point;
        float distance2 = dot(fullDirection, fullDirection);
        float attenuation = 5.0 / distance2;
        vec3 lightDir = normalize(fullDirection);
        
        float intensity = clamp(dot(lightDir, normal), 0.0, 1.0) * attenuation;

        // Diffuse component
        vec3 diffuse = vec3(1.0, 1.0, 1.0) * intensity;

        // Specular component
        vec3 reflectedLightDirection = reflect(lightDir, normal);
        float specularAmount = pow(max(dot(rayDirection, reflectedLightDirection), 0.0), (0.05 * 128.0) * 4.0) * attenuation;
        vec3 specular = vec3(1.0, 1.0, 1.0) * specularAmount;

        float shadowAmount = shadow(point, normal, lightDir);
        vec3 color = (diffuse + specular) * shadowAmount;

        return color;
    }

    vec3 lighting(DirectionalLight light, vec3 rayDirection, vec3 point, vec3 normal) {
        vec3 fullDirection = -light.direction;
        vec3 lightDir = normalize(fullDirection);
        
        float intensity = clamp(dot(lightDir, normal), 0.0, 1.0) * 0.5;

        // Diffuse component
        vec3 diffuse = vec3(1.0, 1.0, 1.0) * intensity;

        // Specular component
        vec3 reflectedLightDirection = reflect(lightDir, normal);
        float specularAmount = pow(max(dot(rayDirection, reflectedLightDirection), 0.0), (0.05 * 128.0) * 4.0);
        vec3 specular = vec3(1.0, 1.0, 1.0) * specularAmount;

        float shadowAmount = shadow(point, normal, lightDir);
        vec3 color = (diffuse + specular) * shadowAmount;

        return color;
    }

    float ambient_occlusion(vec3 position, vec3 normal) {
        // Naive solution: Causes the object to glow, even when there are regions where the plane is below the object
        //return 1.0 - steps / float(RAYMARCH_STEPS - 5.0);

        float sum = 0.0;
        float denominator = 0.0;

        for (int i = 0; i < AMBIENT_OCCLUSION_STEPS; i++) {
            vec3 p = position + normal * (i + 1.0) * AMBIENT_OCCLUSION_STEP_SIZE;
            sum += 1.0 / pow(2.0, i) * get_global_distance(p);
            denominator += 1.0 / pow(2.0, i) * (i + 1.0) * AMBIENT_OCCLUSION_STEP_SIZE;
        }
        sum /= denominator;

        return sum;
    }

    vec3 lighting_total(vec3 rayDirection, vec3 point, vec3 normal) {
        vec3 sum = vec3(0.0);

        sum += lighting(sun, rayDirection, point, normal);
        sum += lighting(light1, rayDirection, point, normal);
        sum += lighting(light2, rayDirection, point, normal);
        sum *= ambient_occlusion(point, normal);

        return sum;
    }

    ReflectionResult reflections(vec3 rayDirection, vec3 point, vec3 normal) {
        // Reflections
        ReflectionResult result;
        vec3 reflectedColor;

        vec3 startPosition = point + normal * SURFACE_DISTANCE_THRESHOLD * 2.0;
        vec3 startDirection = reflect(rayDirection, normal);

        RaymarchResult reflect = ray_march(startPosition, startDirection);
        vec3 hitPoint = startPosition + startDirection * reflect.distanceTravelled;
        
        if (reflect.hitObject) {
            vec3 hitNormal = get_normal(hitPoint);
            reflectedColor = lighting_total(startDirection, hitPoint, hitNormal);
        } else {
            reflectedColor = get_sky_color(startDirection);
        }

        result.nextHitPoint = hitPoint;
        result.nextDirection = startDirection;
        result.color = reflectedColor;

        return result;
    }

    vec3 reflections_iterative(vec3 startRayDir, vec3 startPoint, vec3 startNormal) {
        // Iterative reflections
        vec3 result = vec3(0.0, 0.0, 0.0);
        vec3 rayDir = startRayDir;
        vec3 point = startPoint;
        vec3 normal = startNormal;

        for (int i = 0; i < REFLECTION_ITERATIONS; i++) {
            ReflectionResult reflection = reflections(rayDir, point, normal);

            result += reflection.color;
            rayDir = reflection.nextDirection;
            point = reflection.nextHitPoint;
            normal = get_normal(point);
        }
        result /= (REFLECTION_ITERATIONS * 2.0);

        return result;
    }

    
    void main() {
        vec2 centered = vTCoords * 2.0 - 1.0;
        vec3 pointNDC = vec3(centered.xy, -1.0);
        vec4 pointHomogenuous = vec4(pointNDC, 1.0);
        vec4 eyeDirection = inverse(projection) * pointHomogenuous;
        eyeDirection.w = 0.0;

        vec3 pos = cameraPosition;
        vec3 dir = normalize((inverse(view) * eyeDirection).xyz);

        vec4 color;

        RaymarchResult march = ray_march(pos, dir);
        if (march.hitObject) { 
            vec3 contactPoint = pos + dir * march.distanceTravelled;
            vec3 normal = get_normal(contactPoint);
            
            color = vec4(lighting_total(dir, contactPoint, normal) + reflections_iterative(dir, contactPoint, normal), 1.0);
        } else {
            vec3 skyColor = get_sky_color(dir);

            color = vec4(skyColor, 1.0);
        }

        outColor = color;
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

class RaymarchingDemo : public ic::Application {
    ic::Shader screenShader, rayShader;
    ic::Framebuffer framebuffer;
    
    ic::Mesh2D screenQuad;
    ic::Camera3D camera;
    float time;

    ic::FreeRoamCameraController3D controller;

    public:
        bool init() override {
            displayName = "Raymarching Demo";
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            rayShader = ic::ShaderLoader::load(rayVertex, rayFragment);
            screenShader = ic::ShaderLoader::load(shaders.meshShaderVertex2D, screenFragment);
            
            // Initialize these values directly
            rayShader.use();
            rayShader.set_uniform_vec3f("spheres[0].center", { -1.0f, 0.2f, 0.2f });
            rayShader.set_uniform_color("spheres[0].diffuse", ic::Colors::red);
            rayShader.set_uniform_float("spheres[0].radius", 0.5f);

            rayShader.set_uniform_vec3f("spheres[1].center", { 0.9f, -0.8f, 1.5f });
            rayShader.set_uniform_color("spheres[1].diffuse", ic::Colors::green);
            rayShader.set_uniform_float("spheres[1].radius", 0.35f);

            rayShader.set_uniform_vec3f("spheres[2].center", { -0.4f, -0.8f, 1.5f });
            rayShader.set_uniform_color("spheres[2].diffuse", ic::Colors::cyan);
            rayShader.set_uniform_float("spheres[2].radius", 0.5f);


            rayShader.set_uniform_vec3f("boxes[0].center", { 0.9f, 0.3f, 4.0f });
            rayShader.set_uniform_vec3f("boxes[0].sizes", { 0.2f, 1.0f, 0.7f });

            rayShader.set_uniform_vec3f("boxes[1].center", { 4.0f, 0.1f, 2.0f });
            rayShader.set_uniform_vec3f("boxes[1].sizes", { 0.2f, 0.2f, 0.2f });

            framebuffer = ic::Framebuffer(ic::TEXTURE_ATTACH_COLOR_0, ic::TEXTURE_RGBA, RAYMARCHING_WIDTH, RAYMARCHING_HEIGHT);

            screenQuad = ic::GeometryGenerator::generate_rectangle_mesh(1.0f, 1.0f);
            
            camera = ic::Camera3D();
            controller = ic::FreeRoamCameraController3D(&camera);
            controller.flying = true;

            time = 0.0f;

            return true;
        }

        void window_size_changed(int w, int h) override {
            framebuffer.resize(w, h);
        }

        bool handle_event(ic::Event event, float dt) override { 
            
            return true;
        }

        bool update(float dt) override {
            time += dt;
            
            controller.act(dt);
            camera.update();


            // Rendering
            clear_color(ic::Colors::blue);
            
            framebuffer.use();
            rayShader.use();
            camera.upload_to_shader(rayShader);
            rayShader.set_uniform_vec3f("sun.direction", { 0.2f, cos(time * 0.05f), sin(time * 0.05f) });
            rayShader.set_uniform_vec3f("cameraPosition", camera.position);
            rayShader.set_uniform_vec3f("spheres[0].center", { -1.0f + cos(time), 0.2f + sin(time), 0.2f + sin(time) * cos(time) });
            
            screenQuad.draw(rayShader);
            framebuffer.unuse();


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
    RaymarchingDemo application;

    if (application.construct(RAYMARCHING_WIDTH, RAYMARCHING_HEIGHT)) {
        application.start();
    }

    return 0;
}