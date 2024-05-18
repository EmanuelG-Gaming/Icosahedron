#include <Application.h>

#include <filesystem>
#include <iostream>
#include <algorithm>

#include <Icosahedron/audio/Audio.h>
#include <Icosahedron/util/Noise.h>

#include <Icosahedron/graphics/ImageIO.h>
#include <Icosahedron/math/Mathf.h>



int IC_WINDOW_WIDTH = 0;
int IC_WINDOW_HEIGHT = 0;
float ic::Time::delta = 0.0f;
float ic::Time::globalDelta = 0.0f;
float ic::Time::deltaMultiplier = 1.0f;
float ic::Time::globalTime = 0.0f;
float ic::Time::time = 0.0f;
Uint32 ic::Time::lastUpdate = 0;

using namespace ic;


SDL_GLContext ic::create_GL_context(SDL_Window *window) {
    return SDL_GL_CreateContext(window);
}

/////////////////////
////// Window  //////
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

    if (this->windowHandle) {
        SDL_SetWindowIcon(this->windowHandle, surface);
    }

    SDL_FreeSurface(surface);
}

void ic::Window::set_size(int w, int h) {
    this->width = w;
    this->height = h;
    IC_WINDOW_WIDTH = this->width;
    IC_WINDOW_HEIGHT = this->height;

    if (this->windowHandle) {
        SDL_SetWindowSize(this->windowHandle, this->width, this->height);
        glViewport(0, 0, this->width, this->height);
    }

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

    if (this->windowHandle) { 
        SDL_SetWindowFullscreen(this->windowHandle, flags);
        SDL_SetWindowResizable(this->windowHandle, (to == ic::WindowScaling::resizeable) ? SDL_TRUE : SDL_FALSE);
    }

    this->scaling = to;
}

void ic::Window::set_vsync(int interval) {
    if (SDL_GL_SetSwapInterval(interval) != 0) {
		std::cerr << "SDL_GL_SetSwapInterval Error: " << SDL_GetError() << "\n";
		return;
	}
}

void ic::Window::swap_buffers() {
    if (this->windowHandle) SDL_GL_SwapWindow(this->windowHandle);
}



void ic::Window::set_title(const char *title) {
    this->displayName = title;
    if (this->windowHandle) {
        SDL_SetWindowTitle(this->windowHandle, title);
    }
}

void ic::Window::set_title(const std::string &title) {
    this->set_title(title.c_str());
}


void ic::Window::set_cursor_visibility(bool to) {
    this->cursorVisibility = to;

    if (SDL_ShowCursor(to) < 0) {
		std::cerr << "SDL_ShowCursor Error: " << SDL_GetError() << "\n";
		return;
	}
}

void ic::Window::set_cursor_lock(bool to) {
    if (SDL_SetRelativeMouseMode((SDL_bool) to) < 0) {
        std::cerr << "SDL_SetRelativeMouseMode Error: " << SDL_GetError() << "\n";
        return;
    }
}



void ic::Window::set(const char *title, int w, int h) {
    this->set_title(title);
    this->set_size(w, h);
}
void ic::Window::set(int w, int h) {
    this->set_size(w, h);
}


void ic::Window::init() {
    Uint32 flags = 0;
    flags |= SDL_WINDOW_OPENGL;
    flags |= SDL_WINDOW_SHOWN;

    IC_WINDOW_WIDTH = this->width;
    IC_WINDOW_HEIGHT = this->height;


    this->windowHandle = SDL_CreateWindow(this->displayName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, flags);
	if (this->windowHandle == NULL) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        return;
    }
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

const char *ic::Window::get_title() const {
    return this->displayName;
}

 
SDL_Window *ic::Window::get_handle() const {
    return this->windowHandle;
}
ic::WindowScaling ic::Window::get_scaling() const {
    return this->scaling;
}

void ic::Window::create_GL_context() {
    SDL_GLContext cont = ic::create_GL_context(this->windowHandle);
    if (!cont) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << "\n";
        return;
    } 

    this->glContext = cont;
}



//////////////////////////
///////// Engine /////////
//////////////////////////


Engine::Engine() {
    this->init();
}

void ic::Engine::init() {
    this->set_current_working_directory();
    
    SDL_SetMainReady();
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError();
		return;
	}

    this->set_GL_attributes();
}

void ic::Engine::construct_window() {
    this->window.init();
}

void ic::Engine::create_GL_context() {
    this->window.create_GL_context();

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Couldn't initialize GLAD.\n");
    }

    // Debugging
    this->send_application_information();
}




void ic::Engine::send_application_information() {
    std::string version = std::string((char*) glGetString(GL_VERSION));
    std::string sub = version.substr(0, 5);


    GLint size = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);

    std::cout << "Graphics library -> OpenGL driver compactibility: " << sub << " / " << glGetString(GL_VENDOR) << " / " << glGetString(GL_RENDERER) << "\n";
    std::cout << "Graphics library -> Targeting OpenGL version 3.3" << "\n";
    std::cout << "Graphics library -> Maximum texture size: " << size << "\n";
}


void ic::Engine::set_GL_attributes() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// We want at least 8 bits per color
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}



void ic::Engine::window_tweaks() {
    SDL_GL_SetSwapInterval(1); // Enables VSync
    glViewport(0, 0, this->window.get_width(), this->window.get_height());
}

void ic::Engine::swap_buffers() {
    this->window.swap_buffers();
}

void ic::Engine::tick() {
    ic::Time::tick(SDL_GetTicks());
}




int ic::Engine::poll_events(ic::Event &e) {
    return SDL_PollEvent(&e);
}

bool ic::Engine::process_window_callbacks(ic::Event &e) {
    ic::InputHandler::handle(e);

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
                break;
            }

            if (scaling == WindowScaling::resizeable) {
                this->window.set_size(e.window.data1, e.window.data2);
            } else if (scaling == WindowScaling::fullscreen) {
                SDL_Rect displayRectangle;
                SDL_GetDisplayBounds(0, &displayRectangle);

                this->window.set_size(displayRectangle.w, displayRectangle.h);
            }
            
            //window_size_changed(this->window.get_width(), this->window.get_height());
    }

    return true;
}


void ic::Engine::close() {
    this->window.dispose();

    IMG_Quit();
    SDL_Quit();
}

void ic::Engine::set_current_working_directory() {
    std::string currentDir = std::string(SDL_GetBasePath());
    std::replace(currentDir.begin(), currentDir.end(), '\\', '/');

    // Don't erase the build directory if there's no build directory, obviously
    std::size_t findResult = currentDir.find("/build");
    if (findResult != std::string::npos) {
        currentDir.erase(findResult);
    }

    std::cout << "Current working directory: " << currentDir << "\n";
    
    std::filesystem::current_path(currentDir);
}

///////////////
/// Application
///////////////

/*
Application::Application() {
}

Application::Application(ic::Engine *engine) {
    this->engine = engine;
}

void ic::Application::construct(int w, int h) {
    engine->window.set(w, h);
    this->pre_load();
}
void ic::Application::construct(const char *title, int w, int h) {
    engine->window.set(title, w, h);
    this->pre_load();
}

void ic::Application::pre_load() {
    engine->construct();

    if (!this->init()) {
        return;
    }

    ic::FreeType::load();
    ic::Audio::init();
    ic::Noise::init();
}


void ic::Application::start() {
    if (!this->load()) {
        return;
    }

    bool enabled = true;
    ic::Event event;
    while (enabled) {
        while (engine->poll_events(event)) {
            if (!this->handle_event(event)) {
                enabled = false;
                break;
            }

            if (!engine->process_window_callbacks(event)) {
                enabled = false;
                break;
            }
        }

        ic::Audio::update();
        ic::InputHandler::update();
        
        if (!this->update()) {
            enabled = false;
            break;
        }

        engine->window.swap_buffers();
	    engine->tick();
    }


    ic::FreeType::dispose();
    ic::Audio::dispose();

    this->dispose();
    engine->close();
}
*/
