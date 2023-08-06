#ifndef IC_SHADERS_H
#define IC_SHADERS_H

#include <Icosahedron/graphics/gl/Shader.h>

namespace ic {
    /* OpenGL shader presets. */
    class Shaders {
public:
        /* A simple shader that doesn't use textures. */
        ic::Shader *basicShader2D;
        /* A simple, textured shader. */
        ic::Shader *basicTextureShader2D; 

        void load_shaders() {
            basicShader2D = new ic::Shader(
                "#version 330 core\n"
                "layout (location = 0) in vec2 position;\n"
                "layout (location = 1) in vec3 color;\n"
                "layout (location = 2) in vec2 tCoords;\n"
                "out vec3 vColor;\n"
                "void main(){\n"
                "vColor = color;\n"
                "gl_Position = vec4(position.xy, 0.0, 1.0);\n"
                "}\n",

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
                "#version 330 core\n"
                "layout (location = 0) in vec2 position;\n"
                "layout (location = 1) in vec3 color;\n"
                "layout (location = 2) in vec2 tCoords;\n"
                "out vec3 vColor;\n"
                "out vec2 vTCoords;\n"
                "void main(){\n"
                "vColor = color;\n"
                "vTCoords = tCoords;\n"
                "gl_Position = vec4(position.xy, 0.0, 1.0);\n"
                "}\n",

                "#version 330 core\n"
                "precision mediump float;\n"
                "in vec3 vColor;\n"
                "in vec2 vTCoords;\n"
                "uniform sampler2D sampleTexture;\n"
                "out vec4 outColor;\n"
                "void main(){\n"
                "outColor = texture(sampleTexture, vTCoords) * vec4(vColor, 1.0);\n"
                "}\n", 

                false
            );
        }
    };
}

#endif