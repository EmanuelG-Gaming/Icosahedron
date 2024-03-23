#include <Icosahedron/graphics/gl/Batch.h>
#include <Icosahedron/Global.h>

using namespace ic;


Batch::Batch() {
    this->vertexCapacity = 128;
    this->verticesUsed = 0;
    this->vbo = this->vao = 0;

    this->renderType = ic::INVALID_PRIMITIVE;
}

Batch::Batch(int capacity, GLPrimitives renderType) {
    this->vertexCapacity = capacity;
    this->verticesUsed = 0;
    this->vbo = this->vao = 0;

    this->renderType = renderType;


    if (IC_IS_OPENGL_CONTEXT_PRESENT) {
        setup();
    }
}

void Batch::setup() {
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
    glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(color);

    GLint texCoords = 2;
    glVertexAttribPointer(texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(texCoords);
            
    glBindVertexArray(0);
    glDisableVertexAttribArray(position);
    glDisableVertexAttribArray(color);
    glDisableVertexAttribArray(texCoords);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*
void Batch::add_attribute(int attributeIndex, int dimensions) {
    glBindVertexArray(this->vao);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, this->vertexCapacity * dimensions * sizeof(GLfloat), nullptr, GL_STREAM_DRAW); 


    glVertexAttribPointer(attributeIndex, dimensions, GL_FLOAT, GL_FALSE, 0, (void*)(0));
    glEnableVertexAttribArray(attributeIndex);

    this->bufferObjects.push_back(vertexBuffer);
    this->attributeDimensions.push_back(dimensions);
}
*/

void Batch::add(const std::vector<BatchVertex> &vertices) {
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

void Batch::render() {
    if (this->renderType == INVALID_PRIMITIVE) return;
    if (verticesUsed == 0) {
        return;
    }
    
    glBindVertexArray(this->vao);
    glDrawArrays(this->renderType, 0, verticesUsed);
           
    this->verticesUsed = 0;
}
       
int Batch::get_extra_vertices() {
    if (this->renderType == INVALID_PRIMITIVE) return 0;

    bool mode = (this->renderType == TRIANGLE_STRIPS && verticesUsed > 0);
    return mode ? 2 : 0;
}

void Batch::dispose() {
    if (this->vbo) glDeleteBuffers(1, &this->vbo);
    if (this->vao) glDeleteBuffers(1, &this->vao);
}