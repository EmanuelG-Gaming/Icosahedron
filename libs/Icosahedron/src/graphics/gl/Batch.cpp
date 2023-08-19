#include <Icosahedron/graphics/gl/Batch.h>

using namespace ic;

Batch2D::Batch2D(int capacity, GLPrimitives renderType) {
    this->vertexCapacity = capacity;
    this->indicesUsed = 0;
    this->vao = 0;
    this->ibo = 0;

    this->renderType = renderType;
    
    setup();
}


void Batch2D::setup() {
    if (!this->vao) {
        glGenVertexArrays(1, &this->vao);
    }

    use();
    
    this->set_index_buffer();
}

void Batch2D::add_vertex_data(int vertexBufferIndex, std::vector<float> &vertices) {
    
    int extra = this->get_extra_vertices();

    // @todo Maybe check for vector dimensions?
    if (vertices.size() + extra > vertexCapacity - indicesUsed) {
        return;
    }
    if (vertices.empty()) {
        return;
    }
    if (vertices.size() > vertexCapacity) {
        return;
    }

    BatchVertexBuffer buf = bufferObjects.at(vertexBufferIndex);
    GLuint vertexBuffer = buf.vbo;
    

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    if (extra > 0) {
        glBufferSubData(GL_ARRAY_BUFFER, (buf.verticesUsed + 0) * sizeof(GLfloat), sizeof(GLfloat), &buf.lastUsed);
        glBufferSubData(GL_ARRAY_BUFFER, (buf.verticesUsed + 1) * sizeof(GLfloat), sizeof(GLfloat), &vertices[0]);
    }
    glBufferSubData(GL_ARRAY_BUFFER, buf.verticesUsed * sizeof(GLfloat), vertices.size() * sizeof(GLfloat), &vertices[0]);
    

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    buf.lastUsed = vertices.back();
    buf.verticesUsed += vertices.size() + extra;
    std::cout << buf.verticesUsed << "\n";
}

void Batch2D::add_vertex_data(int vertexBufferIndex, std::vector<ic::Color> &colors) {
    std::vector<float> colorValues;
    for (int i = 0; i < colors.size(); i++) {
         ic::Color color = colors[i];

         colorValues.push_back(color.r / 255.0f);
         colorValues.push_back(color.g / 255.0f);
         colorValues.push_back(color.b / 255.0f);
    }
    add_vertex_data(vertexBufferIndex, colorValues);
}

void Batch2D::add_indices(std::vector<unsigned int> &indices) {
    int extra = this->get_extra_vertices();

    if (indices.size() + extra > vertexCapacity - indicesUsed) {
        return;
    }
    if (indices.empty()) {
        return;
    }
    if (indices.size() > vertexCapacity) {
        return;
    }

    unsigned int maxIndex = indices[0];
    for (int i = 0; i < indices.size(); i++) {
        unsigned int index = indices[i];
        if (index > maxIndex) {
            maxIndex = index;
        }
    }
    maxIndex += 1;

    for (int i = 0; i < indices.size(); i++) {
        this->storedIndices.push_back(indices[i] + this->currentIndexOffset);
        //std::cout << this->storedIndices.at(i + indicesUsed) << " ";
    }
    //std::cout << "\n";

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, this->storedIndices.size() * sizeof(GLuint), this->storedIndices.data());
    //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0 * sizeof(GLuint), this->storedIndices.size() * sizeof(GLuint), this->storedIndices.data());
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //std::cout << this->storedIndices.size() << "\n";

    this->indicesUsed += indices.size() + extra;
    this->currentIndexOffset += maxIndex;
}

void Batch2D::jump() {
    lastVertexBufferPosition++;
}


void Batch2D::add_vertex_buffer(int dimensions) {
    use();

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCapacity * dimensions * sizeof(GLfloat), nullptr, GL_STREAM_DRAW); 
    
    int position = lastVertexBufferPosition;
    glVertexAttribPointer(position, dimensions, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
    glEnableVertexAttribArray(position);

    bufferObjects.push_back(BatchVertexBuffer(dimensions, vertexBuffer));
    lastVertexBufferPosition++;
}

void Batch2D::set_index_buffer() {
    use();

    glGenBuffers(1, &this->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->vertexCapacity * sizeof(GLuint), nullptr, GL_STREAM_DRAW); 
}


void Batch2D::render() {
    if (this->renderType == INVALID_PRIMITIVE) return;
    if (this->indicesUsed == 0) {
        return;
    }
    
    use();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glDrawElements(ic::LINES, this->indicesUsed, GL_UNSIGNED_INT, nullptr);

    this->indicesUsed = 0;
    this->currentIndexOffset = 0;
    this->storedIndices.clear();
}
       

int Batch2D::get_extra_vertices() {
    if (this->renderType == INVALID_PRIMITIVE) return 0;

    bool mode = (this->renderType == TRIANGLE_STRIPS && indicesUsed > 0);
    return mode ? 2 : 0;
}



void Batch2D::dispose() {
    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &ibo);
    for (int i = 0; i < bufferObjects.size(); i++) {
        GLuint vbo = bufferObjects[i].vbo;
        glDeleteBuffers(1, &vbo);
    }

    reset();
}
void Batch2D::reset() {
    bufferObjects.clear();
    storedIndices.clear();
    lastVertexBufferPosition = 0;
    indicesUsed = 0;
    vao = 0;
    ibo = 0;
}



void Batch2D::use() {
    glBindVertexArray(vao);
}

void Batch2D::unuse() {
    glBindVertexArray(0);
}

void Batch2D::unuse_attribute_definitions() {
    unuse();
    for (int i = 0; i < bufferObjects.size(); i++) {
        glDisableVertexAttribArray(i);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}