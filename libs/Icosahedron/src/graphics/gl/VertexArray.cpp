#include <Icosahedron/graphics/gl/VertexArray.h>

using namespace ic;

///// Drawable /////

VertexArrayDrawable::VertexArrayDrawable(GLuint vao, GLsizei indicesUsed) : vao(vao), indicesUsed(indicesUsed) {
}

void ic::VertexArrayDrawable::use() {
    glBindVertexArray(vao);
}
void ic::VertexArrayDrawable::draw(GLPrimitives primitive) {
    glDrawElements(primitive, indicesUsed, GL_UNSIGNED_INT, nullptr);
}

void ic::VertexArrayDrawable::use_and_draw(GLPrimitives primitive) {
    use();
    draw(primitive);
}


///// Vertex array /////

VertexArray::VertexArray() {
    this->setup();
}


ic::VertexArrayDrawable ic::VertexArray::get_drawable() {
    return ic::VertexArrayDrawable(vao, indicesUsed);
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
    lastVertexBufferPosition = 0;
    vao = 0;
    ibo = 0;
    indicesUsed = 0;
}


void ic::VertexArray::jump() {
    lastVertexBufferPosition++;
}
void ic::VertexArray::add_vertex_buffer(int dimensions, const std::vector<GLfloat> &content) {
    use();

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, content.size() * sizeof(GLfloat), content.data(), GL_STATIC_DRAW); 
    
    int position = lastVertexBufferPosition;
    glVertexAttribPointer(position, dimensions, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
    glEnableVertexAttribArray(position);

    bufferObjects.push_back(vertexBuffer);
    lastVertexBufferPosition++;
}
void ic::VertexArray::set_index_buffer(const std::vector<GLuint> &content) {
    use();

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, content.size() * sizeof(GLuint), content.data(), GL_STATIC_DRAW); 

    ibo = indexBuffer;
    indicesUsed = content.size();
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