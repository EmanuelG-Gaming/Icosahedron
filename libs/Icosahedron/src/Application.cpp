#include <Application.h>

#include <filesystem>
#include <iostream>

#include <Icosahedron/audio/Audio.h>
#include <Icosahedron/util/Noise.h>

#include <Icosahedron/graphics/ImageIO.h>
#include <Icosahedron/graphics/gl/Shaders.h>
#include <Icosahedron/math/Mathf.h>


int IC_WINDOW_WIDTH = 0;
int IC_WINDOW_HEIGHT = 0;
bool IC_IS_OPENGL_CONTEXT_PRESENT = false;

void ic::Application::clear_color() {
    this->clear_color(0.0f, 0.0f, 0.0f);
}

void ic::Application::clear_color(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_Color;
}

void ic::Application::clear_color(const ic::Color &color) {
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;
    
    this->clear_color(r, g, b);
}


ic::Image ic::Application::take_screenshot(int x, int y, int width, int height) {
    ic::Image result = ic::Image(width, height);

    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, result.data());

    return result;
}
ic::Image ic::Application::take_screenshot() {
    return this->take_screenshot(0, 0, this->width, this->height);
}


void ic::Application::set_window_image(ic::Image image) {
    if (this->window == NULL) {
        printf("Couldn't change the window's image. Window needs to be loaded first!\n");
        return;
    }

    SDL_Surface *surface = ic::ImageIO::get().to_surface(image);
    SDL_SetWindowIcon(this->window, surface);

    SDL_FreeSurface(surface);
    image.dispose();
}

void ic::Application::set_window_size(int w, int h) {
    if (this->window == NULL) {
        printf("Couldn't change the window's size. Window needs to be loaded first!\n");
        return;
    }
    
    this->width = w;
    this->height = h;
    IC_WINDOW_WIDTH = this->width;
    IC_WINDOW_HEIGHT = this->height;

    SDL_SetWindowSize(this->window, this->width, this->height);
    glViewport(0, 0, this->width, this->height);
    window_size_changed(this->width, this->height);
}

void ic::Application::set_window_scaling(ic::WindowScaling to) {
    if (this->window == NULL) {
        printf("Couldn't change the window's scaling. It needs to be loaded first!\n");
        return;
    }

    if (to == WindowScaling::invalid) {
        return;
    }

    uint32_t flags = 0;
    switch (to) {
        case ic::WindowScaling::fullscreen:
            flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
            break;
        
        case ic::WindowScaling::fullscreenClamped:
            flags = SDL_WINDOW_FULLSCREEN;
            break;
        
        default:
            break;
    }

    SDL_SetWindowFullscreen(this->window, flags);
    SDL_SetWindowResizable(this->window, (to == ic::WindowScaling::resizeable) ? SDL_TRUE : SDL_FALSE);

    this->scaling = to;
}

void ic::Application::set_window_vsync(int interval) {
    if (SDL_GL_SetSwapInterval(interval) != 0) {
		std::cerr << "SDL_GL_SetSwapInterval Error: " << SDL_GetError();
		return;
	}
}

void ic::Application::set_window_title(const char *title) {
    if (this->window == NULL) {
        printf("Couldn't change the window's title. It needs to be loaded first!\n");
        return;
    }

    SDL_SetWindowTitle(this->window, title);
}

void ic::Application::set_window_title(const std::string &title) {
    this->set_window_title(title.c_str());
}





bool ic::Application::construct(int w, int h) {
    SDL_SetMainReady();
    
    this->width = w;
    this->height = h;
    this->window = NULL;

    IC_WINDOW_WIDTH = width;
    IC_WINDOW_HEIGHT = height;


    this->set_current_working_directory();
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError();
		return false;
	}

    this->set_window_attributes();
    
    if (!this->init()) {
        std::cerr << "Couldn't initialize the application." << "\n";
        return false;
    }

    this->prepare_window();
    
    this->pre_load();

    this->constructed = true;
    return true;
}

void ic::Application::start() {
    if (!this->constructed) {
        std::cerr << "Couldn't start the application. It wasn't constructed first." << "\n";
        return;
    }

    if (!load()) {
        std::cerr << "Couldn't load the application." << "\n";
        return;
    }

    Uint32 lastUpdate = SDL_GetTicks();
    SDL_Event e;
    bool disabled = false;
    while (!disabled) {
        if (!this->poll_events(e)) {
            break;
        }

        // Update and render to screen code
        if (!this->update(delta)) {
            break;
        }

        // Swap buffers
	    SDL_GL_SwapWindow(this->window);
        
        Uint32 current = SDL_GetTicks();
        this->delta = (current - lastUpdate) / 1000.0f;
        lastUpdate = current;
	}

    this->close();
}




void ic::Application::send_application_information() {
    std::string version = std::string((char*) glGetString(GL_VERSION));
    std::string sub = version.substr(0, 5);


    GLint size = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);

    std::cout << "Graphics library -> OpenGL driver compactibility: " << sub << " / " << glGetString(GL_VENDOR) << " / " << glGetString(GL_RENDERER) << "\n";
    std::cout << "Graphics library -> Targeting OpenGL version 3.3" << "\n";
    std::cout << "Graphics library -> Maximum texture size: " << size << "\n";
}


void ic::Application::set_window_attributes() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// We want at least 8 bits per color
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}


void ic::Application::prepare_window() {
    Uint32 flags = 0;
    if (this->scaling != ic::WindowScaling::invalid && this->scaling != ic::WindowScaling::fixed) {
        flags |= (SDL_WindowFlags) this->scaling;
    }
    flags |= SDL_WINDOW_OPENGL;
    flags |= SDL_WINDOW_SHOWN;


    SDL_Window *win = SDL_CreateWindow(this->displayName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if (win == NULL) {
		auto errorMessage = "SDL_CreateWindow Error: " + std::string(SDL_GetError()) + "\n";
        throw std::runtime_error(errorMessage);
	}
    

	// We will not actually need a context created, but we should create one
	SDL_GLContext cont = SDL_GL_CreateContext(win);
    this->window = win;
    this->context = cont;
    IC_IS_OPENGL_CONTEXT_PRESENT = true;
}

void ic::Application::pre_load() {
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Couldn't initialize GLAD.\n");
    }

    SDL_GL_SetSwapInterval(1); // Enables VSync
    glViewport(0, 0, this->width, this->height);

    if (this->hideCursor) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    
    
    shaders.load_shaders();
    ic::FreeType::get().load();
    ic::Audio::get().init();
    ic::Noise::get().init();

    this->send_application_information();
}


bool ic::Application::poll_events(ic::Event &e) {
    SDL_PumpEvents();
    while (SDL_PollEvent(&e)) {
        ic::InputHandler::get().handle(e, delta);

        if (!this->handle_event(e, delta)) {
            return false;
        }

        
        switch (e.type) {
            case SDL_QUIT: 
                return false;

            case SDL_KEYUP:
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    return false;
                }

            case SDL_WINDOWEVENT:
                bool resized = e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED;
                if (scaling == WindowScaling::fixed || !resized) {
                    continue;
                }

                if (scaling == WindowScaling::resizeable) {
                    set_window_size(e.window.data1, e.window.data2);
                } else if (scaling == WindowScaling::fullscreen) {
                    SDL_Rect displayRectangle;
                    SDL_GetDisplayBounds(0, &displayRectangle);

                    set_window_size(displayRectangle.w, displayRectangle.h);
                }
        }
    }
    
    ic::InputHandler::get().update(delta);

    return true;
}

void ic::Application::close() {
    std::cout << displayName << " exited." << "\n\n";

    this->dispose();

    ic::FreeType::get().dispose();
    ic::Audio::get().dispose();

    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(context);

    IMG_Quit();
    SDL_Quit();
}

void ic::Application::set_current_working_directory() {
    std::string dir = std::string(SDL_GetBasePath());
    std::replace(dir.begin(), dir.end(), '\\', '/');

    // Don't erase the build directory if there's no build directory, obviously
    std::size_t findResult = dir.find("/build");
    if (findResult != std::string::npos) {
        dir.erase(findResult);
    }

    std::cout << dir << "\n";
    
    std::filesystem::current_path(dir);
}

int ic::Application::screen_width() {
    return this->width;
}
int ic::Application::screen_height() {
    return this->height;
}
