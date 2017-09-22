#include "Window.h"
#include <iostream>

Window::Window(const std::string &title, const int width, const int height) : _title(title), _width(width), _height(height) {
	_closed = !init();
}

Window::~Window() {
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
	SDL_Quit();
	_window = nullptr;
	_context = nullptr;
}

bool Window::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Failed to initialize SDL.\n";
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL);

	if (_window == nullptr) {
		std::cerr << "Failed to create window.\n";
		return false;
	}

	_context = SDL_GL_CreateContext(_window);

	//SDL_GL_MakeCurrent(_window, _context);

	if (_context == nullptr) {
		std::cerr << "Failed to create context.\n";
		return false;
	}

	//glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cerr << "Failed to initialize GLEW. \n";
		return false;
	}

	glViewport(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	return true;
}

void Window::doubleBuffer() {
	SDL_GL_SwapWindow(_window);
}
void Window::clear() const {

}
