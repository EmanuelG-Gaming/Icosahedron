#ifndef IC_FRAMEBUFFER_H
#define IC_FRAMEBUFFER_H

#include <glad/glad.h>

#include <Icosahedron/graphics/gl/GL.h>

namespace ic {
    struct FramebufferData {
        int width = 0, height = 0;
        ic::GLTextureAttachments textureAttachment = ic::TEXTURE_ATTACH_COLOR_0;
        ic::GLTextureColorChannels internalFormat = ic::TEXTURE_RGBA;
        ic::GLTextureColorChannels outputFormat = ic::TEXTURE_RGBA;
        ic::GLTextureTypes textureType = ic::T2D;

        FramebufferData() {}
        FramebufferData(int width, int height) : width(width), height(height) {
        }
    };

    /** @brief Wraps an OpenGL framebuffer. */
    class Framebuffer {
        public:
            /** @brief Initializes a new framebuffer, with a read-write format to be used by the shader.
             *  @param channel The read-write format.
            */

            Framebuffer(ic::GLTextureAttachments attachment, ic::GLTextureColorChannels channel, int width, int height);

            /** @brief Initializes a new framebuffer, with two separate texture formats.
             *  @param internalFormat The format that's read by the shader.
             *  @param outputFormat The format that's shown to the screen.
            */
            Framebuffer(ic::GLTextureAttachments attachment, ic::GLTextureColorChannels internalFormat, ic::GLTextureColorChannels outputFormat, int width, int height);
            
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