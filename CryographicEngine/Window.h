#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <SDL.h>
#include <glew.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define FPS 120

class Window {

public:
	Window();
	Window(const std::string &title);
	~Window();

	void clear() const;

	inline bool isClosed() const { return _closed; }

private:
	bool init();

private:
	std::string _title;

	bool _closed = false;

	SDL_Window *_window = nullptr;
	SDL_GLContext _context = nullptr;

};

#endif