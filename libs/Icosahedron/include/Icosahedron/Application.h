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

#include <Icosahedron/graphics/gl/GL.h>
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

    const int TOTAL_WINDOWS = 5;

    struct Window {
        public:
            const char *get_title() const;
            int get_width() const;
            int get_height() const;

            void set(const char *title, int w, int h);
            void set(int w, int h);
            void init();

            void dispose();

            void set_header_image(const ic::Image &image);
            void set_size(int w, int h);
            void set_scaling(ic::WindowScaling to);

            /** @brief Sets the swap interval for the window's OpenGL context.
             *  By default, the application already uses normal vsync.
             *  @param interval 0 for updating the screen immediately,
             *                  1 for updates syncronized with the screen's refresh rate,
             *                  -1 for adaptive vsync
             *          
            */
            void set_vsync(int interval);

            void swap_buffers();


            void set_title(const char *title);
            void set_title(const std::string &title);

            void set_cursor_visibility(bool to);
            void set_cursor_lock(bool to);




            /** @brief Takes an image of a rectangular portion of the viewport. */
            ic::Image take_screenshot(int x, int y, int width, int height);

            /** @brief Takes an image of the whole viewport. */
            ic::Image take_screenshot();

            SDL_Window *get_handle() const;
            ic::WindowScaling get_scaling() const;


        protected:
            /** Whether or not the cursor is hidden. Note that the mouse's motion will still be registered in the game loop,
             *  even if the cursor is hidden. */
            bool cursorVisibility = true;
        
            /* The name that is displayed on the window. */
            const char *displayName = "test";

            /* The type of scaling that this window uses. */
            ic::WindowScaling scaling = ic::WindowScaling::fixed;

            SDL_Window *windowHandle = NULL;
            SDL_GLContext glContext;
            
            /** @brief Window dimensions, expressed in physical screen pixels. */
            int width = 640, height = 480;
            int windowIndex;

    };


    class Engine {
        public:
            ic::Window window;

            /* Sets up an SDL window along with an OpenGL context. */
            bool construct();

            void close();

            void tick();

            int poll_events(ic::Event &e);
            bool process_window_callbacks(ic::Event &e);
            void swap_buffers();
            
        private:
            /* Sends relevant information such as the current OpenGL, GLEW, and SDL contexts' versions. */
            void send_application_information();

            void set_window_attributes();

            /* Called before load(). */
            void pre_load(int w, int h);
    
            /** @brief Sets the app's working directory.
             *  @note The working directory would not be the generated "build" folder.
            */
            void set_current_working_directory();
    };

    class Application {
        public:
            void construct(const char *title, int w, int h);
            void construct(int w, int h);

            void start();

            virtual bool init() { return true; }
            virtual bool update() { return true; }
            virtual bool load() { return true; }
            virtual bool handle_event(ic::Event event) { return true; }
            virtual void window_size_changed(int w, int h) {}
            virtual void dispose() {}
        
        private:
            void pre_load();
            
        protected:
            ic::Engine engine;
    };
}

#endif