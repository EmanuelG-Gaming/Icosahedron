#include <Icosahedron/graphics/gl/Framebuffer.h>

using namespace ic;

Framebuffer::Framebuffer(ic::GLTextureAttachments attachment, int width, int height) {
    this->width = width;
    this->height = height;
    this->textureAttachment = attachment;

    this->setup(this->width, this->height);
}

void ic::Framebuffer::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}

void ic::Framebuffer::unuse() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ic::Framebuffer::use_texture() {
    glBindTexture(GL_TEXTURE_2D, this->textureIndex);
}

void ic::Framebuffer::dispose() {
    if (this->fbo) glDeleteFramebuffers(1, &this->fbo);
    if (this->rbo) glDeleteRenderbuffers(1, &this->rbo);
    if (this->textureIndex) glDeleteTextures(1, &this->textureIndex);
}

void ic::Framebuffer::resize(int w, int h) {
    this->dispose();
    this->fbo = 0;
    this->rbo = 0;
    this->textureIndex = 0;

    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);


    glGenTextures(1, &this->textureIndex);
    glBindTexture(GL_TEXTURE_2D, this->textureIndex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);


    glGenRenderbuffers(1, &this->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);


    glFramebufferTexture2D(GL_FRAMEBUFFER, this->textureAttachment, GL_TEXTURE_2D, this->textureIndex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer is not complete!\n";
        return;
    }

    this->unuse();
}
            
void ic::Framebuffer::setup(int w, int h) {
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);


    glGenTextures(1, &this->textureIndex);
    glBindTexture(GL_TEXTURE_2D, this->textureIndex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);


    glGenRenderbuffers(1, &this->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);


    glFramebufferTexture2D(GL_FRAMEBUFFER, this->textureAttachment, GL_TEXTURE_2D, this->textureIndex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer is not complete!\n";
        return;
    }

    this->unuse();
}