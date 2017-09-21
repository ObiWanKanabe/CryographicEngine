#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Window.h"
#include "OpenGLRenderer.h"

class GameEngine {
private:
	GameEngine();
	static GameEngine *theInstance;
	static Window *window;
public:
	~GameEngine();
	static GameEngine& getInstance();
	inline static Window& getWindow() { return *window; };
	bool onStart();
	void onEnd();
	void preRender();
	void render();
	void postRender();
	void logMessage();

};

#endif

