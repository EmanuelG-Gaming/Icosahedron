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
    namespace Shaders {
        const std::string basicShaderVertex2D = IC_ADD_GLSL_DEFINITION(
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
        
        const std::string basicShaderFrag2D = IC_ADD_GLSL_DEFINITION(
            precision mediump float;
        
            in vec3 vColor;
            out vec4 outColor;
        
            void main() {
                outColor = vec4(vColor, 1.0);
            }
        );
        
        
        const std::string basicTextureShaderVertex2D = basicShaderVertex2D;
        const std::string basicTextureShaderFrag2D = IC_ADD_GLSL_DEFINITION(
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
        
        const std::string basicTextShaderVertex2D = basicShaderVertex2D;
        const std::string basicTextShaderFrag2D = IC_ADD_GLSL_DEFINITION(
            precision mediump float;
        
            in vec3 vColor;
            in vec2 vTCoords;
        
            uniform sampler2D sampleTexture;
        
            out vec4 outColor;
        
            void main() {
                vec4 color = texture(sampleTexture, vTCoords);
                if (color.r <= 0.5) discard;
                outColor = vec4(color.r, color.r, color.r, 1.0) * vec4(vColor, 1.0);
            }
        );
        
        const std::string meshShaderVertex2D = IC_ADD_GLSL_DEFINITION(
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
        
        const std::string meshShaderFrag2D = IC_ADD_GLSL_DEFINITION(
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
                vec4 textureColor = texture(sampleTexture, vTCoords);
                if (textureColor.a <= 0.1) discard;
                
                vec4 first = vec4(vColor, 1.0) * textureColor;
                if (first.rgb == vec3(0.0, 0.0, 0.0)) first = textureColor;
                if (first.rgb == vec3(0.0, 0.0, 0.0)) first = vec4(vColor, 1.0);
                
                vec4 c = mix(first, vec4(material.baseColor, 1.0), material.colorBlending);
                outColor = c;
            }
        );
        
        
        const std::string meshShaderVertex3D = IC_ADD_GLSL_DEFINITION(
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

        // ----- Skyboxes -----

        // A modified ic::Shaders::meshShaderVertex3D that removes the translational component (mat4 -> mat3 -> mat4 again) of the camera's view matrix,
        // thus fixing the skybox in place
        const std::string skyboxVertex = IC_ADD_GLSL_DEFINITION(
            layout (location = 0) in vec3 position;
        
            uniform mat4 projection = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
            uniform mat4 view = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
            uniform mat4 model = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
            
            out vec3 vPosition;
        
            void main() {
                mat4 viewNoTranslation = mat4(mat3(view));
                vec4 pos = model * vec4(position, 1.0);
        
                vPosition = pos.xyz;
                gl_Position = projection * viewNoTranslation * pos;
            }
        );

        // A basic GLSL fragment code for rendering a skybox that doesn't support gamma correction
        const std::string skyboxFragment = IC_ADD_GLSL_DEFINITION(
            precision mediump float;
        
            in vec3 vPosition;

            uniform samplerCube sampleTexture;
        
            out vec4 outColor;
        
            void main() {
                outColor = texture(sampleTexture, vPosition);
            }
        );
    };
}

#endif