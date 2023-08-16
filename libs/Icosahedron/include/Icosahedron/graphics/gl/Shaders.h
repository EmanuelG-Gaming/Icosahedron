#ifndef IC_SHADERS_H
#define IC_SHADERS_H

#include <string>


#include <Icosahedron/graphics/gl/Shader.h>

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
            basicShaderVertex2D = 
                "#version 330 core\n"
                "layout (location = 0) in vec2 position;\n"
                "layout (location = 1) in vec3 color;\n"
                "layout (location = 2) in vec2 tCoords;\n"
                "uniform mat4 projection;\n"
                "uniform int useCamera;\n"
                "out vec3 vColor;\n"
                "out vec2 vTCoords;\n"
                "void main(){\n"
                "vColor = color;\n"
                "vTCoords = tCoords;\n"
                "vec4 pos = vec4(position, 0.0, 1.0);\n"
                "if (useCamera == 1) {\n"
                "pos = projection * pos;\n"
                "}\n"
                "gl_Position = pos;\n"
                "}\n"; 
           

            basicShaderFrag2D = 
                "#version 330 core\n"
                "precision mediump float;\n"
                "in vec3 vColor;\n"
                "out vec4 outColor;\n"
                "void main(){\n"
                "outColor = vec4(vColor, 1.0);\n"
                "}\n";
        

        
            basicTextureShaderVertex2D = basicShaderVertex2D;
            basicTextureShaderFrag2D =
                "#version 330 core\n"
                "precision mediump float;\n"
                "in vec3 vColor;\n"
                "in vec2 vTCoords;\n"
                "uniform sampler2D sampleTexture;\n"
                "out vec4 outColor;\n"
                "void main(){\n"
                "vec4 color = texture(sampleTexture, vTCoords);\n"
                "if (color.a <= 0.2) discard;\n"
                "outColor = color * vec4(vColor, 1.0);\n"
                "}\n";

            basicTextShaderVertex2D = basicShaderVertex2D;
            basicTextShaderFrag2D =
                "#version 330 core\n"
                "precision mediump float;\n"
                "in vec3 vColor;\n"
                "in vec2 vTCoords;\n"
                "uniform sampler2D sampleTexture;\n"
                "out vec4 outColor;\n"
                "void main(){\n"
                "vec4 color = texture(sampleTexture, vTCoords);\n"
                "if (color.r <= 0.2) discard;\n"
                "outColor = vec4(color.r, color.r, color.r, 1.0) * vec4(vColor, 1.0);\n"
                "}\n";
            
            meshShaderVertex2D = basicShaderVertex2D;
            meshShaderFrag2D =
                "#version 330 core\n"
                "precision mediump float;\n"
                "struct Material {\n"
                "float colorBlending;\n"
                "vec3 baseColor;\n"
                "};\n"
                
                "in vec3 vColor;\n"
                "in vec2 vTCoords;\n"
                "uniform Material material;\n"
                "uniform sampler2D sampleTexture;\n"
                "out vec4 outColor;\n"
                "void main(){\n"
                "vec4 c = mix(vec4(vColor, 1.0), vec4(material.baseColor, 1.0), material.colorBlending);\n"
                "vec4 color = texture(sampleTexture, vTCoords);\n"
                "if (color.a <= 0.2) discard;\n"
                "outColor = color * c;\n"
                "}\n";
        }
    };
}

#endif