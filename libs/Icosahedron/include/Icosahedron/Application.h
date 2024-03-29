#ifndef IC_APPLICATION_H
#define IC_APPLICATION_H

// Add the option to not use parameters in the main function, paired with SDL_SetMainReady()
#define SDL_MAIN_HANDLED

#include <string>

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <Icosahedron/Global.h>

#include <Icosahedron/graphics/Color.h>

#include <Icosahedron/graphics/gl/GLStateHandler.h>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/input/Input.h>
#include <Icosahedron/input/InputHandler.h>

#include <Icosahedron/graphics/FreeType.h>
#include <Icosahedron/graphics/Image.h>
#include <Icosahedron/util/Time.h>
#include <Icosahedron/Renderer.h>


namespace ic {
    enum class WindowScaling {
        invalid = -1,
        
        fixed = 0,
        resizeable = SDL_WINDOW_RESIZABLE,
        fullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP,
        fullscreenClamped = SDL_WINDOW_FULLSCREEN
    };

    class Application {
        public:
            /* Initializes the application. Called before load(), inside the construct() function.
             * Don't use this to add OpenGL stuff, as a GL context doesn't exist during this stage. */
            virtual bool init() { return true; }

            /* Called after setting a window and a valid OpenGL context, inside the start() function. */
            virtual bool load() { return true; }

            virtual bool handle_event(ic::Event event) { return true; }
            virtual void window_size_changed(int w, int h) {}
            
            virtual bool update() { return true; }
            virtual void dispose() {};

            /* Sets up an SDL window along with an OpenGL context. */
            bool construct(int w, int h);
            /* The actual starting point of the application. */
            void start();

            /** @note This would dispose the image. */
            void set_window_image(ic::Image image);
            void set_window_size(int w, int h);
            void set_window_scaling(ic::WindowScaling to);

            /** @brief Sets the swap interval for the window's OpenGL context.
             *  By default, the application already uses normal vsync.
             *  @param interval 0 for updating the screen immediately,
             *                  1 for updates syncronized with the screen's refresh rate,
             *                  -1 for adaptive vsync
             *          
            */
            void set_window_vsync(int interval);

            void set_window_title(const char *title);
            void set_window_title(const std::string &title);

            
            void clear_color(float r, float g, float b);
            void clear_color(const ic::Color &color);
            void clear_color();

            /** @brief Takes an image of a rectangular portion of the viewport. */
            ic::Image take_screenshot(int x, int y, int width, int height);

            /** @brief Takes an image of the whole viewport. */
            ic::Image take_screenshot();
            
            int screen_width();
            int screen_height();
            
        private:
            /* Sends relevant information such as the current OpenGL, GLEW, and SDL contexts' versions. */
            void send_application_information();

            void set_window_attributes();
            void prepare_window();

            /* Called before load(). */
            void pre_load();

            /** @brief Sets the app's working directory.
             *  @note The working directory would not be the generated "build" folder.
            */
            void set_current_working_directory();

            bool poll_events(ic::Event &e);
            
            void close();
            
        protected:
            /* The name that is displayed on the window. */
            std::string displayName = "test";

            /* The type of scaling that this window uses. */
            ic::WindowScaling scaling = ic::WindowScaling::fixed;

            /** Whether or not the cursor is hidden. Note that the mouse's motion would still be registered in the game loop. */
            bool hideCursor = false;
            
        private:
            int width = 0, height = 0;
            
            SDL_Window *window;
            SDL_GLContext context;
            
            bool constructed = false;
    };
}

#endif