#ifndef IC_SHADER_H
#define IC_SHADER_H

#include <string>
#include <glad/glad.h>

#include <Icosahedron/math/geom/Vector.h>
#include <Icosahedron/math/geom/Vectors.h>

#include <Icosahedron/math/Matrices.h>

#include <Icosahedron/graphics/Color.h>


namespace ic {
    class Shader {
        public:
            Shader();
            Shader(std::string vertexContent, std::string fragmentContent);

            void use();
            void clear();

            GLint attribute_location(const std::string &name);
            GLint uniform_location(const std::string &name);

            void set_uniform_int(const std::string &name, int value);
            void set_uniform_bool(const std::string &name, bool value);
            void set_uniform_float(const std::string &name, float value);
            
            void set_uniform_mat3(const std::string &name, const ic::Mat3x3 &mat);
            void set_uniform_mat4(const std::string &name, const ic::Mat4x4 &mat);
            
            void set_uniform_vec2f(const std::string &name, const ic::Vec2f &vec);
            void set_uniform_vec3f(const std::string &name, const ic::Vec3f &vec);
            void set_uniform_vec2f(const std::string &name, float x, float y);
            void set_uniform_vec3f(const std::string &name, float x, float y, float z);
            
            void set_uniform_color(const std::string &name, float r, float g, float b);
            void set_uniform_color(const std::string &name, const ic::Color &color);
            
            void load(const std::string &vertSource, const std::string &fragSource);
            
        protected:
            GLuint program;
    };
}
#endif