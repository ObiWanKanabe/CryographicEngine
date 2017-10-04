#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Window.h"
#include "OpenGLRenderer.h"
#include "GameInterface.h"

// Frames per second for the game
#define FPS 240

class GameEngine {
private:

	// Game Engine is a singleton, therefore private Constructor
	GameEngine();

	// The single instance of the Engine
	static GameEngine *theInstance;

	// The pointers to Window and Renderer used in the engine
	static Window *window;
	static OpenGLRenderer *renderer;

	// Set the game loop to be running
	bool isRunning = true;
public:

	// Default Deconstructor
	~GameEngine();

	// Returns the single instance of the Engine
	static GameEngine& getInstance();

	// Returns the Engine's window
	inline static Window& getWindow() { return *window; };

	// Initialization of the engine done here
	void onStart();

	// Called when the engine is destroyed
	void onEnd();

	// Called before the engine renders
	void preRender();

	// Called while renderering
	void render();

	// Called after the engine renders
	void postRender();

	// Log a message for debug use
	void logMessage();

	//GameInterface gameInterface;

};

#endif

