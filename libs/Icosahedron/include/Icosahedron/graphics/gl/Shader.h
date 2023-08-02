#ifndef IC_SHADER_H
#define IC_SHADER_H

#include <string>

#include <GL/glew.h>

#include <Icosahedron/math/geom/Vector.h>
#include <Icosahedron/graphics/Color.h>


namespace ic {
    class Shader {
        public:
            std::string vertexContent, fragmentContent;

            Shader(std::string vertexFile, std::string fragmentFile, bool loadingFromFile = true);

            void use();
            void clear();

            GLint attribute_location(const std::string &name);
            GLint uniform_location(const std::string &name);

            void set_uniform_int(const std::string &name, int value);
            void set_uniform_bool(const std::string &name, bool value);
            void set_uniform_float(const std::string &name, float value);
            
            void set_uniform_vec2f(const std::string &name, float x, float y);
            void set_uniform_vec3f(const std::string &name, float x, float y, float z);
            
            void set_uniform_color(const std::string &name, float r, float g, float b);
            void set_uniform_color(const std::string &name, const ic::Color &color);

        private:
            void load(const std::string &vertSource, const std::string &fragSource);
            void load_content_from_files(const std::string &vertFile, const std::string &fragFile, std::string &vertContent, std::string &fragContent);

        protected:
            GLuint program;
            std::string vertexFileName, fragmentFileName;
    };
}
#endif