#ifndef IC_SHADERS_H
#define IC_SHADERS_H

#include <string>


#include <Icosahedron/graphics/gl/Shader.h>

namespace ic {
    /* OpenGL shader presets. */
    class Shaders {
        public:

        std::string basicShaderVertex2D;

        /* A simple shader that doesn't use textures. */
        ic::Shader *basicShader2D;

        /* A simple, textured shader. */
        ic::Shader *basicTextureShader2D; 

        /* A shader that is used to render text. */
        ic::Shader *basicTextShader2D;

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

            basicShader2D = new ic::Shader(
                basicShaderVertex2D,

                "#version 330 core\n"
                "precision mediump float;\n"
                "in vec3 vColor;\n"
                "out vec4 outColor;\n"
                "void main(){\n"
                "outColor = vec4(vColor, 1.0);\n"
                "}\n", 

                false
            );

        
            basicTextureShader2D = new ic::Shader(
                basicShaderVertex2D,

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
                "}\n", 

                false
            );

            basicTextShader2D = new ic::Shader(
                basicShaderVertex2D,

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
                "}\n", 

                false
            );
        }
    };
}

#endif