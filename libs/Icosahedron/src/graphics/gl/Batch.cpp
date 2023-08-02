#include <Icosahedron/graphics/gl/Batch.h>

using namespace ic;

Batch2D::Batch2D(int capacity, GLPrimitives renderType) {
    this->shader = new ic::Shader(
        "#version 300 es\n"
        "layout (location = 0) in vec2 position;\n"
        "layout (location = 1) in vec3 color;\n"
        "out vec3 vColor;\n"
        "void main(){\n"
        "vColor = color;\n"
        "gl_Position = vec4(position.xy, 0.0, 1.0);\n"
        "}\n",

        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec3 vColor;\n"
        "out vec4 outColor;\n"
        "void main(){\n"
        "outColor = vec4(vColor, 1.0);\n"
        "}\n", 

        false
    );
    
    this->vertexCapacity = capacity;
    this->verticesUsed = 0;
    this->vbo = this->vao = 0;
    
    this->renderType = renderType;
    this->shader = shader;
           
    setup();
}

void Batch2D::setup() {
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
           
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertexCapacity * sizeof(BatchVertex), nullptr, GL_STREAM_DRAW); 
    
    // Attributes
    GLint position = 0;
    glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), (void*)(0));
    glEnableVertexAttribArray(position);
           
    GLint color = 1;
    glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(color);
           
    glBindVertexArray(0);
    glDisableVertexAttribArray(position);
    glDisableVertexAttribArray(color);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Batch2D::add(const std::vector<BatchVertex> &vertices) {
    int extra = this->get_extra_vertices();
    if (vertices.size() + extra > vertexCapacity - verticesUsed) {
        return;
    }
    if (vertices.empty()) {
        return;
    }
    if (vertices.size() > vertexCapacity) {
        return;
    }
           
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    if (extra > 0) {
        glBufferSubData(GL_ARRAY_BUFFER, (verticesUsed + 0) * sizeof(BatchVertex), sizeof(BatchVertex), &lastUsed);
        glBufferSubData(GL_ARRAY_BUFFER, (verticesUsed + 1) * sizeof(BatchVertex), sizeof(BatchVertex), &vertices[0]);
    }
    glBufferSubData(GL_ARRAY_BUFFER, verticesUsed * sizeof(BatchVertex), vertices.size() * sizeof(BatchVertex), &vertices[0]);
            
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    verticesUsed += vertices.size() + extra;
    lastUsed = vertices.back();
}

void Batch2D::render() {
    if (this->renderType == INVALID_PRIMITIVE) return;
    if (verticesUsed == 0) {
        return;
    }
    
    this->shader->use();
    glBindVertexArray(this->vao);
    glDrawArrays(this->renderType, 0, verticesUsed);
           
    this->verticesUsed = 0;
}
       
int Batch2D::get_extra_vertices() {
    if (this->renderType == INVALID_PRIMITIVE) return 0;

    bool mode = (this->renderType == TRIANGLE_STRIPS && verticesUsed > 0);
    return mode ? 2 : 0;
}

void Batch2D::dispose() {
    if (this->vbo) glDeleteBuffers(1, &this->vbo);
    if (this->vao) glDeleteBuffers(1, &this->vao);

    this->shader->clear();
}