#include <Icosahedron/Application.h>
#include <Icosahedron/util/Noise.h>

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


std::string chunkVertexShader = IC_ADD_GLSL_DEFINITION(
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    layout (location = 2) in int tIndices;
    layout (location = 3) in int tCoordIndex;
    layout (location = 4) in vec3 normal;

    uniform mat4 projection = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    uniform mat4 view = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    uniform mat4 model = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    uniform mat4 normalModel = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    
    out vec3 vPosition;
    out vec3 vColor;
    out vec3 vTCoords;
    out vec3 vNormal;

    void main() {
        vec2 coords[4] = vec2[4](
            vec2(0.0, 0.0),
            vec2(1.0, 0.0),
            vec2(1.0, 1.0),
            vec2(0.0, 1.0)
        );

        vec4 pos = model * vec4(position, 1.0);

        vPosition = pos.xyz;
        vColor = color;
        vTCoords = vec3(coords[tCoordIndex], float(tIndices));
        vNormal = vec3(normalModel * vec4(normal, 1.0));
        
        gl_Position = projection * view * pos;
    }
);

std::string chunkFragmentShader = IC_ADD_GLSL_DEFINITION(
    precision mediump float;
    
    in vec3 vPosition;
    in vec3 vTCoords;
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

    struct DirectionalLight {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

    PointLight l = PointLight(
        vec3(0.0, 5.0, 0.0), 

        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.0, 0.0, 0.0), 
        
        1.0, 0.09, 0.032
    );
    PointLight l2 = PointLight(
        vec3(10.0, 5.0, 10.0), 

        vec3(0.2, 0.2, 0.2), 
        vec3(0.9, 0.8, 0.65), 
        vec3(0.0, 0.0, 0.0), 
        
        1.0, 0.09, 0.032
    );

    DirectionalLight d = DirectionalLight(
        vec3(0.0, 5.0, -3.0), 

        vec3(0.2, 0.2, 0.2), 
        vec3(0.5, 0.5, 0.6), 
        vec3(0.1, 0.1, 0.1)
    );

    uniform sampler2DArray sampleTexture;
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
        vec4 color1 = compute_lighting(l);
        vec4 color2 = compute_lighting(l2);
        vec4 color3 = compute_lighting(d);

        outColor = color1 + color2 + color3;
    }
);







const std::size_t CHUNK_WIDTH = 200, CHUNK_HEIGHT = 16, CHUNK_DEPTH = 200;
const std::size_t CHUNK_VOLUME = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;

using chunk_t = uint8_t;
using block_t = uint8_t;


struct BlockFace {
    std::array<float, 12> vertexData;
    ic::Vec3f normal;

    BlockFace(std::array<float, 12> vertexData, ic::Vec3f normal) : vertexData(vertexData), normal(normal) {}
};

namespace BlockFaces {
    const BlockFace left = BlockFace({
        0,1,1,
        0,1,0,
        0,0,0,
        0,0,1
    }, { -1,0,0 });

    const BlockFace right = BlockFace({
        1,1,0,
        1,1,1,
        1,0,1,
        1,0,0
    }, { 1,0,0 });

    const BlockFace bottom = BlockFace({
        0,0,0,
        1,0,0,
        1,0,1,
        0,0,1
    }, { 0,-1,0 });

    const BlockFace top = BlockFace({
        1,1,0,
        0,1,0,
        0,1,1,
        1,1,1
    }, { 0,1,0 });

    const BlockFace back = BlockFace({
        0,1,0,
        1,1,0,
        1,0,0,
        0,0,0
    }, { 0,0,-1 });

    const BlockFace front = BlockFace({
        1,1,1,
        0,1,1,
        0,0,1,
        1,0,1
    }, { 0,0,1 });
};


struct ChunkMeshData {
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<int> textureIndices;
    std::vector<int> textureCoordIndices;
    std::vector<unsigned int> indices;     
};


struct Chunk {
    public:
        ic::Mesh3D mesh;
        std::array<block_t, CHUNK_VOLUME> blocks;
        int meshIndexCount;

        Chunk() {
            memset(&blocks, 0, sizeof(blocks));
            mesh = ic::Mesh3D();
            meshIndexCount = 0;
        }


        void form_mesh() {
            ChunkMeshData data;

            for (int y = 0; y < CHUNK_HEIGHT; y++) {
                for (int z = 0; z < CHUNK_DEPTH; z++) {
                    for (int x = 0; x < CHUNK_WIDTH; x++) {
                        int blockType = blocks[get_index(x, y, z)];

                        if (blockType) {
                            // Check if an air block exists in the proximity of a solid block
    
                            // -x
                            if (!get_block_bounded(x - 1, y, z)) {
                                add_face(BlockFaces::left, data, x, y, z, 0);
                            }
    
                            // +x
                            if (!get_block_bounded(x + 1, y, z)) {
                                add_face(BlockFaces::right, data, x, y, z, 0);
                            }
    
                            // -y
                            if (!get_block_bounded(x, y - 1, z)) {
                                add_face(BlockFaces::bottom, data, x, y, z, 0);
                            }
    
                            // +y
                            if (!get_block_bounded(x, y + 1, z)) {
                                add_face(BlockFaces::top, data, x, y, z, 1);
                            }
    
                            // -z
                            if (!get_block_bounded(x, y, z - 1)) {
                                add_face(BlockFaces::back, data, x, y, z, 0);
                            }
    
                            // +z
                            if (!get_block_bounded(x, y, z + 1)) {
                                add_face(BlockFaces::front, data, x, y, z, 0);
                            }
                        }
                    }
                }
            }

            mesh.add_attribute(0, 3, data.positions);
            mesh.add_attribute(2, 1, data.textureIndices);
            mesh.add_attribute(3, 1, data.textureCoordIndices);
            mesh.add_attribute(4, 3, data.normals);
            mesh.set_index_buffer(data.indices);
        }


        void add_face(BlockFace face, ChunkMeshData &data, chunk_t x, chunk_t y, chunk_t z, int textureIndex) {
            // Vertex data
            for (int i = 0; i < 4; i++) {
                float px = x + face.vertexData[i * 3 + 0];
                float py = y + face.vertexData[i * 3 + 1];
                float pz = z + face.vertexData[i * 3 + 2];
                ic::Vec3f normal = face.normal;
                

                data.positions.push_back(px);
                data.positions.push_back(py);
                data.positions.push_back(pz);

                data.normals.push_back(normal.x());
                data.normals.push_back(normal.y());
                data.normals.push_back(normal.z());


                data.textureIndices.push_back(textureIndex);
                data.textureCoordIndices.push_back(i);
            }

            // Indices
            std::vector<unsigned int> offsets = {
                0, 1, 2,
                2, 3, 0
            };

            for (int i = 0; i < 6; i++) {
                data.indices.push_back(offsets[i] + meshIndexCount);
            }

            meshIndexCount += 4;
        }

        int get_block_bounded(chunk_t x, chunk_t y, chunk_t z) {
            if (x < 0 || y < 0 || z < 0 ||
                x >= CHUNK_WIDTH || y >= CHUNK_HEIGHT || z >= CHUNK_DEPTH) return 0;

            return blocks[get_index(x, y, z)];
        }

        int get_index(chunk_t x, chunk_t y, chunk_t z) {
            return (int)(y * (CHUNK_WIDTH * CHUNK_DEPTH) + z * CHUNK_HEIGHT + x);
        }


        void draw(ic::Shader &shader) {
            mesh.draw(shader);
        }

        void dispose() {
            mesh.dispose();
        }
};



class GLCraft : public ic::Application {
    ic::Shader chunkShader;
    ic::TextureArray textureArray;

    ic::Camera3D camera;
    ic::FreeRoamCameraController3D controller;
    Chunk chunk;

    float time;

    public:
        bool init() override {
            displayName = "GLCraft";
            hideCursor = true;

            return true;
        }
        
        bool load() override {
            time = 0.0f;

            states.enable_depth_testing(ic::LESS);
            states.enable_face_culling(ic::FRONT, ic::CCW);
            
            
            chunkShader = ic::ShaderLoader::get().load(chunkVertexShader, chunkFragmentShader);
            textureArray = ic::TextureArray(64, 4);
            textureArray.add_texture("resources/textures/dirt.png");
            textureArray.add_texture("resources/textures/grass.png");

            camera = ic::Camera3D();
            camera.position = { -3.0f, 1.5f, 0.0f };

            controller = ic::FreeRoamCameraController3D(&camera);
            controller.flying = true;
            controller.speed = 10.0f;


            chunk = Chunk();
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                for (int z = 0; z < CHUNK_DEPTH; z++) {
                    float height = ic::Noise::get().perlin_2D(x / 10.0f, z / 10.0f, true) * 8.0f;
                    
                    for (int y = 0; y <= height; y++) {
                        chunk.blocks[chunk.get_index(x, y, z)] = 1;
                    }
                }
            }

            chunk.form_mesh();

            
            return true;
        }

        void window_size_changed(int w, int h) override {
            
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
            chunkShader.use();
            chunkShader.set_uniform_vec3f("viewPosition", camera.position);
            camera.upload_to_shader(chunkShader);

            textureArray.use();
            chunk.draw(chunkShader);

           
            return true; 
        }

        void dispose() override {
            textureArray.dispose();
            chunkShader.clear();
            chunk.dispose();
        }
};

int main() {
    GLCraft application;

    if (application.construct(640, 480)) {
        application.start();
    }

    return 0;
}