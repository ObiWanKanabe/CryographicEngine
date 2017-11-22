#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <SDL.h>
#include <glew.h>

class Window {

public:
	
	// Constructor takes a title, width & height
	Window(const std::string &title, const int width, const int height);

	// Default Deconstructor
	~Window();

	// Clear the window to black
	void Clear() const;

	// Swap buffers
	void DoubleBuffer();

	// Set SDL/OpenGL window attributes
	void SetAttributes();

	// Returns if the window is closed
	inline bool IsClosed() const { return _closed; }

	// Returns width
	inline int GetWidth() const { return _width; }

	// Returns height
	inline int GetHeight() const { return _height; }

private:

	// Initializes Window resources and attributes
	bool Init();

private:

	// Title of the window
	std::string _title;

	// Width and Height of the window in pixels
	int _width, _height;

	// By default the window is not closed
	bool _closed = false;

	// Window and Context pointers
	SDL_Window *_window = nullptr;
	SDL_GLContext _context = nullptr;

};

#endif