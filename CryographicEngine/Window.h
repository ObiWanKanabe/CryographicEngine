#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <SDL.h>
#include <glew.h>

#define FPS 120

class Window {

public:
	
	// Constructor takes a title, width & height
	Window(const std::string &title, const int width, const int height);

	~Window();

	// Clear the window to black
	void clear() const;

	// Swap buffers
	void doubleBuffer();

	// Set SDL/OpenGL window attributes
	void setAttributes();

	inline bool isClosed() const { return _closed; }
	inline int getWidth() const { return _width; }
	inline int getHeight() const { return _height; }

private:
	bool init();

private:
	std::string _title;
	int _width;
	int _height;
	bool _closed = false;

	SDL_Window *_window = nullptr;
	SDL_GLContext _context = nullptr;

};

#endif