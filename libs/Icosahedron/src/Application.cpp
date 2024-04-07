#include <Application.h>

#include <filesystem>
#include <iostream>

#include <Icosahedron/audio/Audio.h>
#include <Icosahedron/util/Noise.h>

#include <Icosahedron/graphics/ImageIO.h>
#include <Icosahedron/math/Mathf.h>


int IC_WINDOW_WIDTH = 0;
int IC_WINDOW_HEIGHT = 0;
bool IC_IS_OPENGL_CONTEXT_PRESENT = false;
float ic::Time::delta = 0.0f;
float ic::Time::globalDelta = 0.0f;
float ic::Time::deltaMultiplier = 1.0f;
float ic::Time::globalTime = 0.0f;
float ic::Time::time = 0.0f;

/////////////////////
//// Window /////////
/////////////////////

ic::Image ic::Window::take_screenshot(int x, int y, int width, int height) {
    ic::Image result = ic::Image(width, height);
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, result.data());

    return result;
}

ic::Image ic::Window::take_screenshot() {
    return this->take_screenshot(0, 0, this->width, this->height);
}


void ic::Window::set_header_image(const ic::Image &image) {
    SDL_Surface *surface = ic::ImageIO::to_surface(image);
    SDL_SetWindowIcon(this->windowHandle, surface);

    SDL_FreeSurface(surface);
}

void ic::Window::set_size(int w, int h) {
    this->width = w;
    this->height = h;
    IC_WINDOW_WIDTH = this->width;
    IC_WINDOW_HEIGHT = this->height;

    SDL_SetWindowSize(this->windowHandle, this->width, this->height);
    glViewport(0, 0, this->width, this->height);

    // TODO: Event-based programming
    //window_size_changed(this->width, this->height);
}

void ic::Window::set_scaling(ic::WindowScaling to) {
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

    SDL_SetWindowFullscreen(this->windowHandle, flags);
    SDL_SetWindowResizable(this->windowHandle, (to == ic::WindowScaling::resizeable) ? SDL_TRUE : SDL_FALSE);

    this->scaling = to;
}

void ic::Window::set_vsync(int interval) {
    if (SDL_GL_SetSwapInterval(interval) != 0) {
		std::cerr << "SDL_GL_SetSwapInterval Error: " << SDL_GetError();
		return;
	}
}

void ic::Window::set_title(const char *title) {
    this->displayName = title;
    SDL_SetWindowTitle(this->windowHandle, title);
}

void ic::Window::set_title(const std::string &title) {
    this->set_title(title.c_str());
}


void ic::Window::set_cursor_visibility(bool to) {
    this->cursorVisibility = to;

    if (SDL_ShowCursor(to) < 0) {
		std::cerr << "SDL_ShowCursor Error: " << SDL_GetError();
		return;
	}
}

void ic::Window::set_cursor_lock(bool to) {
    SDL_SetRelativeMouseMode((SDL_bool) to);
}

void ic::Window::init(int w, int h) {
    Uint32 flags = 0;
    flags |= SDL_WINDOW_OPENGL;
    flags |= SDL_WINDOW_SHOWN;

    this->width = w;
    this->height = h;
    IC_WINDOW_WIDTH = this->width;
    IC_WINDOW_HEIGHT = this->height;


    this->windowHandle = SDL_CreateWindow(this->displayName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, flags);
	if (this->windowHandle == NULL) {
		auto errorMessage = "SDL_CreateWindow Error: " + std::string(SDL_GetError()) + "\n";
        throw std::runtime_error(errorMessage);
	}

    // We will not actually need a context created, but we should create one
	SDL_GLContext cont = SDL_GL_CreateContext(this->windowHandle);
    this->glContext = cont;
    IC_IS_OPENGL_CONTEXT_PRESENT = true;
}


void ic::Window::dispose() {
    std::cout << "Window with name: " << this->displayName << " at index: " << this->windowIndex << " was disposed." << "\n";

    SDL_DestroyWindow(this->windowHandle);
    SDL_GL_DeleteContext(this->glContext);
}


int ic::Window::get_width() const {
    return this->width;
}

int ic::Window::get_height() const {
    return this->height;
}

SDL_Window *ic::Window::get_handle() const {
    return this->windowHandle;
}
ic::WindowScaling ic::Window::get_scaling() const {
    return this->scaling;
}


//////////////////////////
//// Application /////////
//////////////////////////

bool ic::Application::construct(int w, int h) {
    SDL_SetMainReady();
    
    this->set_current_working_directory();
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError();
		return false;
	}

    this->set_window_attributes();
    this->prepare_window(w, h);

    if (!this->init()) {
        std::cerr << "Couldn't initialize the application." << "\n";
        return false;
    }
    
    this->pre_load(w, h);

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
        ic::Time::globalTime += ic::Time::globalDelta;
        ic::Time::time += ic::Time::delta;

        if (!this->poll_events(e)) {
            break;
        }

        ic::Audio::update();
        
        // Update and render to screen code
        if (!this->update()) {
            break;
        }

        // Swap buffers
	    SDL_GL_SwapWindow(this->window.get_handle());
        
        Uint32 current = SDL_GetTicks();
        ic::Time::globalDelta = (current - lastUpdate) / 1000.0f;
        ic::Time::delta = ic::Time::globalDelta * ic::Time::deltaMultiplier; 
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


void ic::Application::prepare_window(int w, int h) {
    window.init(w, h);
}

void ic::Application::pre_load(int w, int h) {
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Couldn't initialize GLAD.\n");
    }

    SDL_GL_SetSwapInterval(1); // Enables VSync
    glViewport(0, 0, w, h);
    
    ic::FreeType::load();
    ic::Audio::init();
    ic::Noise::init();

    this->send_application_information();
}


bool ic::Application::poll_events(ic::Event &e) {
    SDL_PumpEvents();
    while (SDL_PollEvent(&e)) {
        ic::InputHandler::handle(e);

        if (!this->handle_event(e)) {
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
                ic::WindowScaling scaling = this->window.get_scaling();

                bool resized = e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED;
                if (scaling == WindowScaling::fixed || !resized) {
                    continue;
                }

                if (scaling == WindowScaling::resizeable) {
                    this->window.set_size(e.window.data1, e.window.data2);
                } else if (scaling == WindowScaling::fullscreen) {
                    SDL_Rect displayRectangle;
                    SDL_GetDisplayBounds(0, &displayRectangle);

                    this->window.set_size(displayRectangle.w, displayRectangle.h);
                }
                
                window_size_changed(this->window.get_width(), this->window.get_height());
        }
    }
    
    ic::InputHandler::update();

    return true;
}

void ic::Application::close() {
    this->dispose();

    ic::FreeType::dispose();
    ic::Audio::dispose();

    this->window.dispose();

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

    std::cout << "Current working directory: " << dir << "\n";
    
    std::filesystem::current_path(dir);
}