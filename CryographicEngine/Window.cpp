#include "Window.h"
#include <iostream>

Window::Window(const std::string &title, const int width, const int height) : _title(title), _width(width), _height(height) {
	_closed = !Init();
}

Window::~Window() {
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
	IMG_Quit();
	SDL_Quit();
	_window = nullptr;
	_context = nullptr;
	delete _window;
	delete _context;
}

bool Window::Init() {

	// Initialize SDL and error check
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "Failed to initialize SDL.\n";
		return false;
	}

	//Initialize SDL Image and error check
	int imgFlags = IMG_INIT_PNG|IMG_INIT_JPG|IMG_INIT_TIF|IMG_INIT_WEBP;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cerr << "Failed to initialize SDL_image.\n";
		return false;
	}


	// Create the window and store it 
	_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL);

	// Error check window
	if (_window == nullptr) {
		std::cerr << "Failed to create window.\n";
		return false;
	}

	// Create the context and store it
	_context = SDL_GL_CreateContext(_window);

	// Error check context
	if (_context == nullptr) {
		std::cerr << "Failed to create context.\n";
		return false;
	}

	if (GLEW_OK != glewInit()) {
		std::cerr << "Failed to initialize GLEW.\n";
		return false;
	}

	// Set attributes for SDL/OpenGL window
 	SetAttributes();

	// Clear the window by default black
	Clear();

	return true;
}

void Window::DoubleBuffer() {
	SDL_GL_SwapWindow(_window);
}

void Window::SetAttributes() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	//glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::Clear() const {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
