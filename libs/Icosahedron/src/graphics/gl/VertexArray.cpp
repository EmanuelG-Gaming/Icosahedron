#include <Icosahedron/graphics/gl/VertexArray.h>

using namespace ic;

///// Drawable /////

VertexArrayDrawable::VertexArrayDrawable(GLuint vao, GLuint ibo, GLsizei indicesUsed, bool usingIndices) : vao(vao), ibo(ibo), indicesUsed(indicesUsed), usingIndices(usingIndices) {
}

void ic::VertexArrayDrawable::use() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void ic::VertexArrayDrawable::draw(GLPrimitives primitive) {
    if (usingIndices) {
        glDrawElements(primitive, indicesUsed, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(primitive, 0, indicesUsed);
    }
}

void ic::VertexArrayDrawable::use_and_draw(GLPrimitives primitive) {
    use();
    draw(primitive);
}


///// Vertex array /////

VertexArray::VertexArray() {
}


ic::VertexArrayDrawable ic::VertexArray::get_drawable() {
    return ic::VertexArrayDrawable(vao, ibo, indicesUsed, usingIndices);
}


void ic::VertexArray::use() {
    glBindVertexArray(vao);
}
void ic::VertexArray::unuse() {
    glBindVertexArray(0);
}


void ic::VertexArray::dispose() {
    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(bufferObjects.size(), bufferObjects.data());

    reset();
}

void ic::VertexArray::reset() {
    bufferObjects.clear();
    vao = 0;
    ibo = 0;
    indicesUsed = 0;
}

GLuint ic::VertexArray::vertex_buffer(int attributeIndex, int dimensions, const std::vector<float> &content) {
    use();

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, content.size() * sizeof(GLfloat), content.data(), GL_STATIC_DRAW); 
    
    glVertexAttribPointer(attributeIndex, dimensions, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
    glEnableVertexAttribArray(attributeIndex);
    
    return vertexBuffer;
}


GLuint ic::VertexArray::vertex_buffer(int attributeIndex, int dimensions, const std::vector<int> &content) {
    use();

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, content.size() * sizeof(GLint), content.data(), GL_STATIC_DRAW); 
    
    glVertexAttribIPointer(attributeIndex, dimensions, GL_INT, 0, (GLvoid*)(0));
    glEnableVertexAttribArray(attributeIndex);

    return vertexBuffer;
}

GLuint ic::VertexArray::add_vertex_buffer(int attributeIndex, int dimensions, const std::vector<float> &content) {
    GLuint vbo = vertex_buffer(attributeIndex, dimensions, content);
    bufferObjects.emplace_back(vbo);

    return vbo;
}

GLuint ic::VertexArray::add_vertex_buffer(int attributeIndex, int dimensions, const std::vector<int> &content) {
    GLuint vbo = vertex_buffer(attributeIndex, dimensions, content);
    bufferObjects.emplace_back(vbo);

    return vbo;
}



GLuint ic::VertexArray::set_index_buffer(const std::vector<unsigned int> &content) {
    use();

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, content.size() * sizeof(GLuint), content.data(), GL_STATIC_DRAW); 

    ibo = indexBuffer;
    indicesUsed = content.size();

    return indexBuffer;
}



void ic::VertexArray::unuse_attribute_definitions() {
    unuse();
    for (int i = 0; i < bufferObjects.size(); i++) {
        glDisableVertexAttribArray(i);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void ic::VertexArray::setup() {
    if (!this->vao) {
        glGenVertexArrays(1, &vao);
    }
}


ic::VertexArray &ic::VertexArray::using_indices(bool to) {
    this->usingIndices = to;
    return *this;
}

ic::VertexArray &ic::VertexArray::set_index_count(GLsizei to) {
    this->indicesUsed = to;
    return *this;
}
