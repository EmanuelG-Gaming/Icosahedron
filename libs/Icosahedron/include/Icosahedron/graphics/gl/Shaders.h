#ifndef IC_SHADERS_H
#define IC_SHADERS_H

#include <string>


#include <Icosahedron/graphics/gl/Shader.h>


/** @brief Appends the GLSL version to the shader code.
  * Also removes the need to add quotation marks at each line of code.
*/
#define IC_ADD_GLSL_DEFINITION(string) "#version 330 core\n" #string

namespace ic {

    /** @brief OpenGL shader presets. */
    class Shaders {
        public:
        std::string basicShaderVertex2D,
                    basicShaderFrag2D;

        std::string basicTextureShaderVertex2D,
                    basicTextureShaderFrag2D;

        std::string basicTextShaderVertex2D,
                    basicTextShaderFrag2D;
        
        std::string meshShaderVertex2D,
                    meshShaderFrag2D;

        std::string meshShaderVertex3D;

        void load_shaders() {
            basicShaderVertex2D = IC_ADD_GLSL_DEFINITION(
                layout (location = 0) in vec2 position;
                layout (location = 1) in vec3 color;
                layout (location = 2) in vec2 tCoords;

                uniform mat4 projection = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
                
                out vec3 vColor;
                out vec2 vTCoords;

                void main() {
                    vColor = color;
                    vTCoords = tCoords;
                    vec4 pos = vec4(position, 0.0, 1.0);

                    pos = projection * pos;
                    
                    gl_Position = pos;
                }
            );

            basicShaderFrag2D = IC_ADD_GLSL_DEFINITION(
                precision mediump float;

                in vec3 vColor;
                out vec4 outColor;

                void main() {
                    outColor = vec4(vColor, 1.0);
                }
            );

        
            basicTextureShaderVertex2D = basicShaderVertex2D;
            basicTextureShaderFrag2D = IC_ADD_GLSL_DEFINITION(
                precision mediump float;

                in vec3 vColor;
                in vec2 vTCoords;

                uniform sampler2D sampleTexture;

                out vec4 outColor;

                void main() {
                    vec4 color = texture(sampleTexture, vTCoords);
                    if (color.a <= 0.1) discard;
                    outColor = color * vec4(vColor, 1.0);
                }
            );

            basicTextShaderVertex2D = basicShaderVertex2D;
            basicTextShaderFrag2D = IC_ADD_GLSL_DEFINITION(
                precision mediump float;

                in vec3 vColor;
                in vec2 vTCoords;

                uniform sampler2D sampleTexture;

                out vec4 outColor;

                void main() {
                    vec4 color = texture(sampleTexture, vTCoords);
                    if (color.r <= 0.1) discard;
                    outColor = vec4(color.r, color.r, color.r, 1.0) * vec4(vColor, 1.0);
                }
            );

            meshShaderVertex2D = IC_ADD_GLSL_DEFINITION(
                layout (location = 0) in vec2 position;
                layout (location = 1) in vec3 color;
                layout (location = 2) in vec2 tCoords;

                uniform mat4 projection = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
                uniform mat4 model = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
                
                out vec2 vPosition;
                out vec3 vColor;
                out vec2 vTCoords;

                void main() {
                    vec4 pos = model * vec4(position, 0.0, 1.0);

                    vPosition = pos.xy;
                    vColor = color;
                    vTCoords = tCoords;
                    
                    gl_Position = projection * pos;
                }
            );

            meshShaderFrag2D = IC_ADD_GLSL_DEFINITION(
                precision mediump float;

                struct Material {
                    float colorBlending;
                    vec3 baseColor;
                };

                in vec2 vPosition;
                in vec3 vColor;
                in vec2 vTCoords;
                
                uniform Material material = Material(0.0, vec3(1.0, 1.0, 1.0));
                uniform sampler2D sampleTexture;

                out vec4 outColor;

                void main() {
                    vec4 color = texture(sampleTexture, vTCoords);
                    if (color.a <= 0.1) discard;

                    vec4 c = mix(vec4(vColor, 1.0) * color, vec4(material.baseColor, 1.0), material.colorBlending);
                    
                    outColor = c;
                }
            );


            meshShaderVertex3D = IC_ADD_GLSL_DEFINITION(
                layout (location = 0) in vec3 position;
                layout (location = 1) in vec3 color;
                layout (location = 2) in vec2 tCoords;
                layout (location = 3) in vec3 normal;

                uniform mat4 projection = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
                uniform mat4 view = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
                uniform mat4 model = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
                uniform mat4 normalModel = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
                
                out vec3 vPosition;
                out vec3 vColor;
                out vec2 vTCoords;
                out vec3 vNormal;

                void main() {
                    vec4 pos = model * vec4(position, 1.0);

                    vPosition = pos.xyz;
                    vColor = color;
                    vTCoords = tCoords;
                    vNormal = vec3(normalModel * vec4(normal, 1.0));
                    
                    gl_Position = projection * view * pos;
                }
            );
        }
    };
}

#endif