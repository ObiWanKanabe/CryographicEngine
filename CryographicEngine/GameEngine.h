#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Window.h"
#include "OpenGLRenderer.h"
#include "GameInterface.h"

class GameEngine {
private:
	GameEngine();
	static GameEngine *theInstance;
	static Window *window;
	static OpenGLRenderer *renderer;
	bool isRunning = true;
public:
	~GameEngine();
	static GameEngine& getInstance();
	inline static Window& getWindow() { return *window; };
	void onStart();
	void onEnd();
	void preRender();
	void render();
	void postRender();
	void logMessage();

	//GameInterface gameInterface;

};

#endif

