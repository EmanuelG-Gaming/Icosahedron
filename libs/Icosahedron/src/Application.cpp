#include <Application.h>

#include <iostream>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/math/Mathf.h>

int IC_WINDOW_WIDTH = 0;
int IC_WINDOW_HEIGHT = 0;

void ic::Application::clear_color() {
    clear_color(0.0f, 0.0f, 0.0f);
}

void ic::Application::clear_color(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ic::Application::clear_color(const ic::Color &color) {
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;
    
    clear_color(r, g, b);
}

bool ic::Application::construct(int w, int h) {
    width = w;
    height = h;

    IC_WINDOW_WIDTH = width;
    IC_WINDOW_HEIGHT = height;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "SDL_Init Error: " << SDL_GetError();
		return false;
	}
    
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// We want at least 8 bits per color
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

    if (!init()) {
        std::cerr << "Couldn't initialize the application." << "\n";
        return false;
    }

    Uint32 flags = 0;
    if (scaling == WindowScaling::resizeable) flags |= SDL_WINDOW_RESIZABLE;
    else if (scaling == WindowScaling::fullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    flags |= SDL_WINDOW_OPENGL;

    SDL_Window *win = SDL_CreateWindow(displayName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if (win == NULL)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
		return false;
	}
    
	// We will not actually need a context created, but we should create one
	SDL_GLContext cont = SDL_GL_CreateContext(win);
    window = win;
    context = cont;
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Couldn't initialize GLEW." << "\n";
        return false;
    }
    
    send_application_information();

    pre_load();

    if (!load()) {
        std::cerr << "Couldn't load the application." << "\n";
        return false;
    }
    
    glViewport(0, 0, w, h);
    
    constructed = true;
    return true;
}

void ic::Application::start() {
    if (!constructed) {
        std::cerr << "Couldn't start the application. It wasn't constructed first." << "\n";
        return;
    }

    float delta = 0.0f;
    Uint64 then = SDL_GetPerformanceCounter();
    bool disabled = false;

    SDL_Event e;
    while (!disabled) {
        SDL_PumpEvents();

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    disabled = true;
                    break;
                case SDL_KEYUP:
                    if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                        disabled = true;
                        break;
                    }
                case SDL_WINDOWEVENT:
                    if (scaling != WindowScaling::fixed &&
                       (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)) {
                        
                        int w = 0, h = 0;
                        if (scaling == WindowScaling::resizeable) {
                            w = e.window.data1;
                            h = e.window.data2;
                        } else if (scaling == WindowScaling::fullscreen) {
                            SDL_Rect displayRectangle;
                            SDL_GetDisplayBounds(0, &displayRectangle);

                            w = displayRectangle.w;
                            h = displayRectangle.h;
                        }

                        width = w;
                        height = h;
                        IC_WINDOW_WIDTH = width;
                        IC_WINDOW_HEIGHT = height;
                        
                        glViewport(0, 0, width, height);
                        //std::cout << width << " " << height << "\n";
                    }
            }

            inputHandler.handle(e, delta);

            // Event-handling code
            if (!handle_event(e, delta)) {
                disabled = true;
                break;
            }
        }
        SDL_Delay(10);
        
        Uint64 now = SDL_GetPerformanceCounter();
        delta = (now - then) / (float) SDL_GetPerformanceFrequency();
        then = now;

        inputHandler.update(delta);

    	// Update and render to screen code
    	if (!update(delta)) {
            disabled = true;
        }
    	
		// Swap buffers
		SDL_GL_SwapWindow(window);
	}
	dispose();
	ic::FreeType::get().dispose();
    
	SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(context);

	SDL_Quit();
}

void ic::Application::send_application_information() {
    SDL_version compiled, linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    GLint majorGL, minorGL;
    glGetIntegerv(GL_MAJOR_VERSION, &majorGL);
    glGetIntegerv(GL_MINOR_VERSION, &minorGL);

    std::cout << "----- Icosahedron -----" << "\n\n";
    std::cout << "OpenGL driver compactibility: " << majorGL << "." << minorGL << " / " << glGetString(GL_VENDOR) << "\n";
    std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << "\n";
    fprintf(stdout, "Compiled SDL2 version: %u.%u.%u\n", compiled.major, compiled.minor, compiled.patch);
    fprintf(stdout, "Linked SDL2 version: %u.%u.%u\n", linked.major, linked.minor, linked.patch);
}

void ic::Application::pre_load() {
    shaders.load_shaders();
    ic::FreeType::get().load();
}


int ic::Application::screen_width() {
    return width;
}
int ic::Application::screen_height() {
    return height;
}