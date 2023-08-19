#ifndef IC_TEXTURE_H
#define IC_TEXTURE_H

#include <vector>
#include <string>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <Icosahedron/graphics/gl/GL.h>


namespace ic {
    /** @brief An OpenGL wrapper for a texture.
     * A texture is bound using the use() function, in order to be applied to geometry. */
    template <ic::GLTextureTypes T>
    class Texture {
        public:
            Texture() {}
            Texture(bool smooth, bool repeating) : smooth(smooth), repeating(repeating) {}
            Texture(std::vector<std::string> fileNames, bool smooth = false, bool repeating = true) : Texture(smooth, repeating) {
                setup(fileNames);
            }

            void use() {
                if (textureIndex) {
                    GLenum type = static_cast<GLenum>(T);
                    glBindTexture(type, textureIndex);
                    glActiveTexture(GL_TEXTURE0);
                    
                }
            }
            void unuse() {
                GLenum type = static_cast<GLenum>(T);
                glBindTexture(type, 0);
            }
            void dispose() {
                if (textureIndex) glDeleteTextures(1, &textureIndex);
            }

        protected:
            void setup(const std::vector<std::string> &fileNames) {
                GLenum type = static_cast<GLenum>(T);
              
                glGenTextures(1, &textureIndex);
                glBindTexture(type, textureIndex);

                if (type == GL_TEXTURE_2D) {
                    SDL_Surface *texture = load_surface(fileNames.at(0).c_str());
                    if (texture == NULL) {
                        printf("2D Texture couldn't load.\n");
                        return;
                    }
              
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
                    if (repeating) {
                        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
                        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    } else {
                        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    }
                    SDL_FreeSurface(texture);

                } else if (type == GL_TEXTURE_CUBE_MAP) {
                    for (int i = 0; i < fileNames.size(); i++) {
                        std::string faceName = fileNames.at(i);
                        SDL_Surface *texture = load_surface(faceName.c_str());
                            
                        if (texture == NULL) {
                            printf("Couldn't load the cubemap face/s.\n");
                            return;
                        }
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
                            
                        if (repeating) {
                            glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
                            glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
                            glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_REPEAT);
                        } else {
                            glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                            glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                            glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                        }
                        SDL_FreeSurface(texture);
                    }
                }
                glGenerateMipmap(type);

                if (smooth) {
                    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                } else {
                    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }
              
                glBindTexture(type, 0);
            }

            SDL_Surface *load_surface(const char *path) {
                SDL_Surface *img = IMG_Load(path);
                if (img == NULL) {
                    fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
                    return NULL;
                }
                return img;
            }

        private:
            GLuint textureIndex = 0;
         
            // Texture parameters
            bool repeating = true;
            bool smooth = false;
    };
}

#endif