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

        void load_shaders() {
            basicShaderVertex2D = IC_ADD_GLSL_DEFINITION(
                layout (location = 0) in vec2 position;
                layout (location = 1) in vec3 color;
                layout (location = 2) in vec2 tCoords;

                uniform mat4 projection;
                uniform int useCamera;

                out vec3 vColor;
                out vec2 vTCoords;

                void main() {
                    vColor = color;
                    vTCoords = tCoords;
                    vec4 pos = vec4(position, 0.0, 1.0);

                    if (useCamera == 1) {
                        pos = projection * pos;
                    }

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

                uniform mat4 projection;
                uniform mat4 model = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
                uniform int useCamera;
                
                out vec3 vColor;
                out vec2 vTCoords;

                void main() {
                    vColor = color;
                    vTCoords = tCoords;
                    vec4 pos = model * vec4(position, 0.0, 1.0);

                    if (useCamera == 1) {
                        pos = projection * pos;
                    }

                    gl_Position = pos;
                }
            );

            meshShaderFrag2D = IC_ADD_GLSL_DEFINITION(
                precision mediump float;

                struct Material {
                    float colorBlending;
                    vec3 baseColor;
                };
            
                in vec3 vColor;
                in vec2 vTCoords;

                uniform Material material;
                uniform sampler2D sampleTexture;

                out vec4 outColor;

                void main() {
                    vec4 c = mix(vec4(vColor, 1.0), vec4(material.baseColor, 1.0), material.colorBlending);
                    vec4 color = texture(sampleTexture, vTCoords);
                    if (color.a <= 0.1) discard;

                    outColor = color * c;
                }
            );
        }
    };
}

#endif