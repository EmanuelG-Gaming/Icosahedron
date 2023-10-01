#include <Icosahedron/graphics/gl/Shader.h>

using namespace ic;


Shader::Shader(std::string vertexContent, std::string fragmentContent) {
    this->load(vertexContent, fragmentContent);
}


void Shader::load(const std::string &vertSource, const std::string &fragSource) {
    int check;
    char log[512];
            
    const char *v = vertSource.c_str();
    const char *f = fragSource.c_str();

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &v, NULL);
    glCompileShader(vert);
            
    glGetShaderiv(vert, GL_COMPILE_STATUS, &check); 
    if (!check) {
        glGetShaderInfoLog(vert, 512, NULL, log);
        printf("%s: %s\n", vertSource, log);
    }
            
            
    GLuint fragm = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragm, 1, &f, NULL);
    glCompileShader(fragm);

    glGetShaderiv(fragm, GL_COMPILE_STATUS, &check); 
    if (!check) {
        glGetShaderInfoLog(fragm, 512, NULL, log);
        printf("%s: %s\n", fragSource, log);
    }
            

    this->program = glCreateProgram();
    glAttachShader(this->program, vert);
    glAttachShader(this->program, fragm);
    glLinkProgram(this->program);
               
    glGetProgramiv(this->program, GL_LINK_STATUS, &check);
    if (!check) {
        glGetProgramInfoLog(this->program, 512, NULL, log);
        printf("%s\n", log);
    }

    glDeleteShader(vert);
    glDeleteShader(fragm);
}


void Shader::use() {
    glUseProgram(this->program);
}
void Shader::clear() {
    glDeleteProgram(this->program);
}


GLint Shader::attribute_location(const std::string &name) {
    return glGetAttribLocation(program, name.c_str());
}
GLint Shader::uniform_location(const std::string &name) {
    return glGetUniformLocation(program, name.c_str());
}


void Shader::set_uniform_int(const std::string &name, int value) {
    glUniform1i(this->uniform_location(name), value);
}
void Shader::set_uniform_bool(const std::string &name, bool value) {
    glUniform1i(this->uniform_location(name), (int)value);
}
void Shader::set_uniform_float(const std::string &name, float value) {
    glUniform1f(this->uniform_location(name), value);
}

void Shader::set_uniform_mat3(const std::string &name, const ic::Mat3x3 &mat) {
    glUniformMatrix3fv(this->uniform_location(name), 1, GL_FALSE, mat.values.data());
}
void Shader::set_uniform_mat4(const std::string &name, const ic::Mat4x4 &mat) {
    glUniformMatrix4fv(this->uniform_location(name), 1, GL_FALSE, mat.values.data());
}


void Shader::set_uniform_vec2f(const std::string &name, const ic::Vec2f &vec) {
    this->set_uniform_vec2f(name, vec.values[0], vec.values[1]);
}
void Shader::set_uniform_vec3f(const std::string &name, const ic::Vec3f &vec) {
    this->set_uniform_vec3f(name, vec.values[0], vec.values[1], vec.values[2]);
}
void Shader::set_uniform_vec2f(const std::string &name, float x, float y) {
    glUniform2f(this->uniform_location(name), x, y);
}
void Shader::set_uniform_vec3f(const std::string &name, float x, float y, float z) {
    glUniform3f(this->uniform_location(name), x, y, z);
}


void Shader::set_uniform_color(const std::string &name, float r, float g, float b) {
    glUniform3f(this->uniform_location(name), r, g, b);
}
void Shader::set_uniform_color(const std::string &name, const ic::Color &color) {
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;

    set_uniform_color(name, r, g, b);
}