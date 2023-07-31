#include <Application.h>


void ic::Application::clear_color() {
    clear_color(0.0f, 0.0f, 0.0f);
}

void ic::Application::clear_color(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return false;
	}
    
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// We want at least 8 bits per color
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    

    if (!init()) {
        printf("Couldn't initialize the application.\n");
        return false;
    }

	SDL_Window *win = SDL_CreateWindow(displayName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	if (win == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return false;
	}

	// We will not actually need a context created, but we should create one
	SDL_GLContext cont = SDL_GL_CreateContext(win);
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Couldn't initialize GLEW.\n");
        return false;
    }
    window = win;
    context = cont;

    if (!load()) {
        printf("Couldn't load the application.\n");
        return false;
    }

    constructed = true;
    return true;
}

void ic::Application::start() {
    if (!constructed) {
        printf("Couldn't start the application. It wasn't constructed first.\n");
        return;
    }

    float delta = 0.0f;
    Uint64 then = SDL_GetPerformanceCounter();
    bool disabled = false;

    SDL_Event e;
    while (!disabled)
	{
        while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) {
                 disabled = true;
                 break;
            }

            // Event-handling code
            if (!handle_event(e, delta)) {
                disabled = true;
                break;
            }
		}

        Uint64 now = SDL_GetPerformanceCounter();
        delta = (now - then) / (float) SDL_GetPerformanceFrequency();
        then = now;

    	// Update and render to screen code
    	if (!update(delta)) {
            disabled = true;
        }
    	
		// Swap buffers
		SDL_GL_SwapWindow(window);
	}
	dispose();
	
    SDL_GL_DeleteContext(context);
    
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int ic::Application::screen_width() {
    return width;
}

int ic::Application::screen_height() {
    return height;
}