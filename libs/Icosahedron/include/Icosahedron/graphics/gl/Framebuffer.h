#ifndef IC_FRAMEBUFFER_H
#define IC_FRAMEBUFFER_H

#include <glad/glad.h>

#include <Icosahedron/graphics/gl/GL.h>

namespace ic {
    struct FramebufferData {
        int width = 0, height = 0;
        ic::GLTextureAttachments textureAttachment = ic::TEXTURE_ATTACH_COLOR_0;
        ic::GLTextureColorChannels textureColorChannel = ic::TEXTURE_RGBA;
        ic::GLTextureTypes textureType = ic::T2D;

        FramebufferData() {}
        FramebufferData(int width, int height) : width(width), height(height) {
        }
    };

    /** @brief Wraps an OpenGL framebuffer. */
    class Framebuffer {
        public:
            Framebuffer(ic::GLTextureAttachments attachment, ic::GLTextureColorChannels channel, int width, int height);
            Framebuffer(ic::FramebufferData data);

            void use();
            void unuse();
            void use_texture(int index = 0);
            
            void resize(int w, int h);
            
            void dispose();

        private:
            void setup(int w, int h);

            void initialize_texture(int w, int h);
            void set_texture_content();

        private:
            GLuint fbo;
            GLuint rbo;
            GLuint textureIndex;

            ic::FramebufferData data;
    };
}
#endif