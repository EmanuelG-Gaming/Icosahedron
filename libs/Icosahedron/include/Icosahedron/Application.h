#ifndef IC_APPLICATION_H
#define IC_APPLICATION_H

#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <Icosahedron/graphics/Color.h>
#include <Icosahedron/input/Input.h>

namespace ic {
    class Application {
        public:
            /* Initializes the application. Called before load(). */
            virtual bool init() { return true; }

            /* Called after setting a window and a valid OpenGL context. */
            virtual bool load() { return true; }

            virtual bool handle_event(ic::Event event, float dt) { return true; }
    
            virtual bool update(float dt) { return true; }
            virtual void dispose() {};

            /* Sets up an SDL window along with an OpenGL context. */
            bool construct(int w, int h);
            /* The actual starting point of the application. */
            void start();
            
            void clear_color(float r, float g, float b);
            void clear_color(const ic::Color &color);
            void clear_color();
    
            int screen_width();
            int screen_height();

        protected:
            std::string displayName;

        private:
            int width, height;
            SDL_Window *window;
            SDL_GLContext context;
            bool constructed = false;
    };
}

#endif