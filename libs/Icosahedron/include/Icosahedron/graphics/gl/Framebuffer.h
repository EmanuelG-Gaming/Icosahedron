#ifndef IC_FRAMEBUFFER_H
#define IC_FRAMEBUFFER_H

#include <vector>

#include <glad/glad.h>

#include <Icosahedron/graphics/gl/GL.h>


namespace ic {
    struct FramebufferData {
        int width = 0, height = 0;
        ic::GLTextureTypes textureType = ic::T2D;

        std::vector<ic::GLTextureAttachments> textureAttachemnts;
        std::vector<ic::GLTextureColorChannels> internalFormats, outputFormats;
        std::vector<bool> depthBufferFlags;

        
        FramebufferData() {}
        FramebufferData(int width, int height) : width(width), height(height) {}
    };


    /** @brief Wraps an OpenGL framebuffer. It can use up to 4 rendering targets, a.k.a. fragment shader output color locations. */
    class Framebuffer {
        public:
            Framebuffer();
            
            /** @brief Initializes a framebuffer with an RGBA color target, and depth testing. */
            Framebuffer(int width, int height, const ic::GLTextureTypes &type = ic::T2D);

            /** @brief Initializes a new framebuffer, with a read-write format to be used by the shader.
             *  @param channel The read-write format.
             *  @param hasDepthBuffer Whether or not the shader program will do depth testing.
             *  @param type Self-explanatory.
            */
            Framebuffer(ic::GLTextureAttachments attachment, ic::GLTextureColorChannels channel, int width, int height, bool hasDepthBuffer = true, const ic::GLTextureTypes &type = ic::T2D);

            /** @brief Initializes a new framebuffer, with two separate texture formats.
             *  @param internalFormat The format that's read by the shader.
             *  @param outputFormat The format that's shown to the screen.
             *  @param hasDepthBuffer Whether or not the shader program will do depth testing.
             *  @param type Self-explanatory.
            */
            Framebuffer(ic::GLTextureAttachments attachment, ic::GLTextureColorChannels internalFormat, ic::GLTextureColorChannels outputFormat, int width, int height, bool hasDepthBuffer = true, const ic::GLTextureTypes &type = ic::T2D);


            void use();
            void unuse();
            void use_texture(int index = 0, int indexInTheFragmentShader = 0);
            
            void resize(int w, int h);
            int get_width();
            int get_height();
            
            void add_render_target(const ic::GLTextureAttachments &attachment, const ic::GLTextureColorChannels &internalFormat, const ic::GLTextureColorChannels &outputFormat, bool hasDepthBuffer = true, const ic::GLTextureTypes &type = ic::T2D);
            void add_render_target(const ic::GLTextureAttachments &attachment, const ic::GLTextureColorChannels &format, bool hasDepthBuffer = true, const ic::GLTextureTypes &type = ic::T2D);
             

            void dispose();

        private:
            void add_render_target_raw(const ic::GLTextureAttachments &attachment, const ic::GLTextureColorChannels &internalFormat, const ic::GLTextureColorChannels &outputFormat, bool hasDepthBuffer, const ic::GLTextureTypes &type);

            void initialize_texture(const ic::GLTextureColorChannels &internalFormat, const ic::GLTextureColorChannels &outputFormat, const ic::GLTextureTypes &type, int w, int h);
            void set_texture_content(const ic::GLTextureAttachments &attachment);

        private:
            GLuint fbo;
            GLuint renderBuffers[4];
            GLuint textureIndices[4];
            int lastTextureIndex = 0;
            
            ic::FramebufferData data;
    };
}
#endif