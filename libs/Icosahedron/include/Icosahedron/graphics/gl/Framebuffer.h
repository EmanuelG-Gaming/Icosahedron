#ifndef IC_FRAMEBUFFER_H
#define IC_FRAMEBUFFER_H

#include <GL/glew.h>

#include <Icosahedron/graphics/gl/GL.h>

namespace ic {
    /** @brief Wraps an OpenGL framebuffer. */
    class Framebuffer {
        public:
            Framebuffer(ic::GLTextureAttachments attachment, ic::GLTextureColorChannels channel, int width, int height);

            void use();
            void unuse();
            void use_texture(int index = 0);
            
            void resize(int w, int h);
            
            void dispose();

        private:
            void setup(int w, int h);

        private:
            GLuint fbo;
            GLuint rbo;
            GLuint textureIndex;

            int width, height;
            ic::GLTextureAttachments textureAttachment;
            ic::GLTextureColorChannels textureColorChannel;
    };
}
#endif