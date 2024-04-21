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
Uint32 ic::Time::lastUpdate = 0;

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

void ic::Window::swap_buffers() {
    SDL_GL_SwapWindow(this->get_handle());
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



void ic::Window::set(int w, int h, const char *title) {
    this->displayName = title;
    this->width = w;
    this->height = h;
}

void ic::Window::init() {
    Uint32 flags = 0;
    flags |= SDL_WINDOW_OPENGL;
    flags |= SDL_WINDOW_SHOWN;

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

const char *ic::Window::get_title() const {
    return this->displayName;
}

 
SDL_Window *ic::Window::get_handle() const {
    return this->windowHandle;
}
ic::WindowScaling ic::Window::get_scaling() const {
    return this->scaling;
}


//////////////////////////
///////// Engine /////////
//////////////////////////

bool ic::Engine::construct() {
    this->set_current_working_directory();

    SDL_SetMainReady();
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError();
		return false;
	}

    this->set_window_attributes();
    this->window.init();

    /*
    if (!this->init()) {
        std::cerr << "Couldn't initialize the application." << "\n";
        return false;
    }
    */

    
    this->pre_load(this->window.get_width(), this->window.get_height());

    return true;
}

void ic::Engine::start() {
    /*
    if (!this->constructed) {
        std::cerr << "Couldn't start the application. It wasn't constructed first." << "\n";
        return;
    }

    if (!load()) {
        std::cerr << "Couldn't load the application." << "\n";
        return;
    }
    */

    //ic::Time::lastUpdate = SDL_GetTicks();
    SDL_Event e;
    bool disabled = false;
    while (!disabled) {
        //if (!this->poll_events(e)) {
        //    break;
        //}

        //ic::InputHandler::update();
        //ic::Audio::update();
        
        // Update and render to screen code
        //if (!this->update()) {
        //    break;
        //}

        // Swap buffers
        this->window.swap_buffers();
	    //SDL_GL_SwapWindow(this->window.get_handle());
        
        //ic::Time::tick(SDL_GetTicks());
        this->tick();
	}

    this->close();
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


void ic::Engine::set_window_attributes() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// We want at least 8 bits per color
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}



void ic::Engine::pre_load(int w, int h) {
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

void ic::Engine::swap_buffers() {
    this->window.swap_buffers();
}

void ic::Engine::tick() {
    ic::Time::tick(SDL_GetTicks());
}




int ic::Engine::poll_events(ic::Event &e) {
    //SDL_PumpEvents();
    return SDL_PollEvent(&e);
        //ic::InputHandler::handle(e);

        //if (!this->handle_event(e)) {
        //    return false;
        //}

        //this->poll_events(e);

        //ic::InputHandler::handle(e);

        //if (!this->process_window_callbacks(e)) {
        //    break;
        //}

        /*
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
        */
    //}
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
    //ic::FreeType::dispose();
    //ic::Audio::dispose();
    this->window.dispose();

    IMG_Quit();
    SDL_Quit();
}

void ic::Engine::set_current_working_directory() {
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