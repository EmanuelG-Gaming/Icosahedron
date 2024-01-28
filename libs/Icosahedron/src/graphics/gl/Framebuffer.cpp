#include <Icosahedron/graphics/gl/Framebuffer.h>
#include <Icosahedron/Global.h>

using namespace ic;


Framebuffer::Framebuffer() {
    this->data.width = 8;
    this->data.height = 8;
    this->data.textureType = ic::INVALID_TEXTURE;
}

Framebuffer::Framebuffer(int width, int height, const ic::GLTextureTypes &type) {
    this->data.width = width;
    this->data.height = height;
    this->data.textureType = type;
    
    if (IC_IS_OPENGL_CONTEXT_PRESENT) {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    
        glGenTextures(4, this->textureIndices);
        glGenRenderbuffers(4, this->renderBuffers);
            
        this->add_render_target(ic::TEXTURE_ATTACH_COLOR_0, ic::TEXTURE_RGBA, true, this->data.textureType);
    }
}

Framebuffer::Framebuffer(ic::GLTextureAttachments attachment, ic::GLTextureColorChannels channel, int width, int height, bool hasDepthBuffer, const ic::GLTextureTypes &type) {
    this->data.width = width;
    this->data.height = height;
    this->data.textureType = type;

    if (IC_IS_OPENGL_CONTEXT_PRESENT) {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    
        glGenTextures(4, this->textureIndices);
        glGenRenderbuffers(4, this->renderBuffers);
            
        this->add_render_target(attachment, channel, hasDepthBuffer, this->data.textureType);
    }
}

Framebuffer::Framebuffer(ic::GLTextureAttachments attachment, ic::GLTextureColorChannels internalFormat, ic::GLTextureColorChannels outputFormat, int width, int height, bool hasDepthBuffer, const ic::GLTextureTypes &type) {
    this->data.width = width;
    this->data.height = height;
    this->data.textureType = type;

    if (IC_IS_OPENGL_CONTEXT_PRESENT) {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

        glGenTextures(4, this->textureIndices);
        glGenRenderbuffers(4, this->renderBuffers);

        this->add_render_target(attachment, internalFormat, outputFormat, hasDepthBuffer, this->data.textureType);
    }
}


void ic::Framebuffer::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}

void ic::Framebuffer::unuse() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(this->data.textureType, 0);
}

void ic::Framebuffer::use_texture(int index, int indexInTheFragmentShader) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(this->data.textureType, this->textureIndices[indexInTheFragmentShader]);
}

void ic::Framebuffer::dispose() {
    if (this->fbo) glDeleteFramebuffers(1, &this->fbo);
    glDeleteRenderbuffers(4, this->renderBuffers);
    glDeleteTextures(4, this->textureIndices);
}




void ic::Framebuffer::resize(int w, int h) {
    this->data.width = w;
    this->data.height = h;

    this->fbo = 0;
    memset(this->renderBuffers, 0, sizeof(this->renderBuffers));
    memset(this->textureIndices, 0, sizeof(this->textureIndices));
    this->lastTextureIndex = 0;
    this->dispose();
    
    glGenFramebuffers(1, &this->fbo);


    this->use();
    
    glGenTextures(4, this->textureIndices);
    glGenRenderbuffers(4, this->renderBuffers);

    // Attepts to recover (reload) last used color buffers
    for (int i = 0; i < this->data.textureAttachemnts.size(); i++) {
        this->add_render_target_raw(this->data.textureAttachemnts[i],
                                    this->data.internalFormats[i],
                                    this->data.outputFormats[i], this->data.depthBufferFlags[i], this->data.textureType);
    }

    this->unuse();
}



void ic::Framebuffer::add_render_target_raw(const ic::GLTextureAttachments &attachment, const ic::GLTextureColorChannels &internalFormat, const ic::GLTextureColorChannels &outputFormat, bool hasDepthBuffer, const ic::GLTextureTypes &type) {
    if (this->lastTextureIndex >= 4 - 1) {
        printf("The framebufer attachment count exceeded 4!\n");
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    glBindTexture(this->data.textureType, this->textureIndices[this->lastTextureIndex]);
    this->initialize_texture(internalFormat, outputFormat, type, this->data.width, this->data.height);
    this->set_texture_content(attachment);

    // Add depth to the scene
    if (attachment != GL_DEPTH_ATTACHMENT && hasDepthBuffer) {
        glBindRenderbuffer(GL_RENDERBUFFER, this->renderBuffers[this->lastTextureIndex]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->data.width, this->data.height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->renderBuffers[this->lastTextureIndex]);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    // Convert to raw data
    std::vector<GLenum> attachmentsRaw;
    for (int i = 0; i < this->data.textureAttachemnts.size(); i++) {
        ic::GLTextureAttachments att = this->data.textureAttachemnts[i];
        GLenum a = static_cast<GLenum>(att);
        
        if (a == GL_DEPTH_ATTACHMENT) {
            a = GL_NONE;
        }

        attachmentsRaw.push_back(a);
    }
    

    // Will it be required to specify the color location in the shader?
    glDrawBuffers(this->data.textureAttachemnts.size(), attachmentsRaw.data());
    if (attachment == GL_DEPTH_ATTACHMENT) {
        glReadBuffer(GL_NONE);
    }
    
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer is not complete! Error status: " << status << "\n";
        return;
    }

    this->unuse();

    this->lastTextureIndex++;
}

void ic::Framebuffer::add_render_target(const ic::GLTextureAttachments &attachment, const ic::GLTextureColorChannels &internalFormat, const ic::GLTextureColorChannels &outputFormat, bool hasDepthBuffer,  const ic::GLTextureTypes &type) {
    this->data.textureAttachemnts.push_back(attachment);
    this->data.internalFormats.push_back(internalFormat);
    this->data.outputFormats.push_back(outputFormat);
    this->data.depthBufferFlags.push_back(hasDepthBuffer);

    this->add_render_target_raw(attachment, internalFormat, outputFormat, hasDepthBuffer, type);
}

void ic::Framebuffer::add_render_target(const ic::GLTextureAttachments &attachment, const ic::GLTextureColorChannels &format, bool hasDepthBuffer,  const ic::GLTextureTypes &type) {
    this->add_render_target(attachment, format, format, hasDepthBuffer, type);
}


void ic::Framebuffer::initialize_texture(const ic::GLTextureColorChannels &internalFormat, const ic::GLTextureColorChannels &outputFormat, const ic::GLTextureTypes &type, int w, int h) {
    if (type == ic::T2D) {
        glTexImage2D(this->data.textureType, 0, internalFormat, w, h, 0, outputFormat, GL_FLOAT, NULL);
        glTexParameteri(this->data.textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(this->data.textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    }
    else if (type == ic::TCUBE) {
        for (int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, w, h, 0, outputFormat, GL_FLOAT, NULL);
        }

        glTexParameteri(this->data.textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(this->data.textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(this->data.textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    }


    // Filtering
    glTexParameteri(this->data.textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(this->data.textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    

    // Border color
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(this->data.textureType, GL_TEXTURE_BORDER_COLOR, borderColor);
}


void ic::Framebuffer::set_texture_content(const ic::GLTextureAttachments &attachment) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, this->data.textureType, this->textureIndices[this->lastTextureIndex], 0);
}


void ic::Framebuffer::blit_to_default(int srcX1, int srcY1, int srcX2, int srcY2, int dstX1, int dstY1, int dstX2, int dstY2, GLbitfield mask, GLenum filter) {
    this->blit(0, srcX1, srcY1, srcX2, srcY2, dstX1, dstY1, dstX2, dstY2, mask, filter);
}

void ic::Framebuffer::blit(ic::Framebuffer &to, int srcX1, int srcY1, int srcX2, int srcY2, int dstX1, int dstY1, int dstX2, int dstY2, GLbitfield mask, GLenum filter) {
    this->blit(to.get_index(), srcX1, srcY1, srcX2, srcY2, dstX1, dstY1, dstX2, dstY2, mask, filter);
}

void ic::Framebuffer::blit(GLuint fboIndex, int srcX1, int srcY1, int srcX2, int srcY2, int dstX1, int dstY1, int dstX2, int dstY2, GLbitfield mask, GLenum filter) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo); // Where to read from
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboIndex); // Where to write to

    glBlitFramebuffer(srcX1, srcY1, srcX2, srcY2, dstX1, dstY1, dstX2, dstY2, mask, filter);
}



int ic::Framebuffer::get_width() {
    return this->data.width;
}

int ic::Framebuffer::get_height() {
    return this->data.height;
}

GLuint ic::Framebuffer::get_index() {
    return this->fbo;
}