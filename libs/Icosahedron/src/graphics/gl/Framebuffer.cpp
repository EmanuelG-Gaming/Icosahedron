#include <Icosahedron/graphics/gl/Framebuffer.h>

using namespace ic;

Framebuffer::Framebuffer(ic::GLTextureAttachments attachment, ic::GLTextureColorChannels channel, int width, int height) {
    this->data.width = width;
    this->data.height = height;

    this->data.textureAttachment = attachment;
    this->data.internalFormat = this->data.outputFormat = channel;
    this->data.textureType = ic::T2D;

    this->setup(this->data.width, this->data.height);
}

Framebuffer::Framebuffer(ic::GLTextureAttachments attachment, ic::GLTextureColorChannels internalFormat, ic::GLTextureColorChannels outputFormat, int width, int height) {
    this->data.width = width;
    this->data.height = height;

    this->data.textureAttachment = attachment;
    this->data.internalFormat = internalFormat;
    this->data.outputFormat = outputFormat;
    this->data.textureType = ic::T2D;

    this->setup(this->data.width, this->data.height);
}

Framebuffer::Framebuffer(ic::FramebufferData data) {
    this->data = data;

    this->setup(this->data.width, this->data.height);
}

void ic::Framebuffer::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}

void ic::Framebuffer::unuse() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(this->data.textureType, 0);
}

void ic::Framebuffer::use_texture(int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(this->data.textureType, this->textureIndex);
}

void ic::Framebuffer::dispose() {
    if (this->fbo) glDeleteFramebuffers(1, &this->fbo);
    if (this->rbo) glDeleteRenderbuffers(1, &this->rbo);
    if (this->textureIndex) glDeleteTextures(1, &this->textureIndex);
}

void ic::Framebuffer::resize(int w, int h) {
    this->fbo = 0;
    this->rbo = 0;
    this->textureIndex = 0;
    this->dispose();
    

    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);


    glGenTextures(1, &this->textureIndex);
    glBindTexture(GL_TEXTURE_2D, this->textureIndex);

    this->initialize_texture(w, h);
    glTexParameteri(this->data.textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(this->data.textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(this->data.textureType, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (this->data.textureAttachment != GL_DEPTH_ATTACHMENT) {
        glGenRenderbuffers(1, &this->rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    this->set_texture_content();
    if (this->data.textureAttachment == GL_DEPTH_ATTACHMENT) {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer is not complete!\n";
        return;
    }

    this->unuse();
}
            
void ic::Framebuffer::setup(int w, int h) {
    this->resize(w, h);
}

void ic::Framebuffer::initialize_texture(int w, int h) {
    if (this->data.textureType == ic::T2D) {
        glTexImage2D(this->data.textureType, 0, this->data.internalFormat, w, h, 0, this->data.outputFormat, GL_FLOAT, NULL);
        glTexParameteri(this->data.textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(this->data.textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    } else if (this->data.textureType == ic::TCUBE) {
        for (int i = 0; i < 6; i++) { 
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, this->data.internalFormat, w, h, 0, this->data.outputFormat, GL_FLOAT, NULL);
        }

        glTexParameteri(this->data.textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(this->data.textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(this->data.textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    }
}

void ic::Framebuffer::set_texture_content() {
    glFramebufferTexture2D(GL_FRAMEBUFFER, this->data.textureAttachment, GL_TEXTURE_2D, this->textureIndex, 0);
}