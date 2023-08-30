#ifndef IC_APPLICATION_H
#define IC_APPLICATION_H

#include <string>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <Icosahedron/Global.h>

#include <Icosahedron/graphics/Color.h>

#include <Icosahedron/graphics/gl/GLStateHandler.h>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/input/Input.h>
#include <Icosahedron/input/InputHandler.h>

#include <Icosahedron/graphics/FreeType.h>
#include <Icosahedron/Renderer.h>


namespace ic {
    enum class WindowScaling {
        invalid = -1,
        
        fixed,
        resizeable,
        fullscreen
    };

    class Application {
        public:
            /* Initializes the application. Called before load(). 
             * Don't use this to add OpenGL stuff, as a GL context doesn't exist during this stage. */
            virtual bool init() { return true; }

            /* Called after setting a window and a valid OpenGL context. */
            virtual bool load() { return true; }

            virtual bool handle_event(ic::Event event, float dt) { return true; }
            virtual void window_size_changed(int w, int h) {}
            
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
            
        private:
            /* Sends relevant information such as the current OpenGL, GLEW, and SDL contexts' versions. */
            void send_application_information();

            /* Called before load(). */
            void pre_load();

            /** @brief Sets the app's working directory.
             *  @note The working directory would not be the generated "build" folder.
            */
            void set_current_working_directory();

        protected:
            /* The name that is displayed on the window. */
            std::string displayName = "test";
            /* The type of scaling that this window uses. */
            ic::WindowScaling scaling = ic::WindowScaling::fixed;
            /** Whether or not the cursor is hidden. Note that the mouse's motion would still be there in the game loop. */
            bool hideCursor = false;

            ic::InputHandler inputHandler;
            ic::GLStateHandler states;
            ic::Shaders shaders;
            ic::Renderer renderer;

        private:
            int width = 0, height = 0;
            SDL_Window *window;
            SDL_GLContext context;

            bool constructed = false;
    };
}

#endif