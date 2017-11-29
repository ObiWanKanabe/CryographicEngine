#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Window.h"
#include "OpenGLRenderer.h"
#include "GameInterface.h"
#include "Triangle.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "ImageManager.h"

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

	// Testing in Engine
	Triangle *triangle;
public:

	// Default Deconstructor
	~GameEngine();

	// Returns the single instance of the Engine
	static GameEngine& GetInstance();

	// Returns the Engine's window
	inline static Window& GetWindow() { return *window; };

	// Initialization of the engine done here
	void OnStart();

	// Called when the engine is destroyed
	void OnEnd();

	// Called before the engine renders
	void PreRender();

	// Called while renderering
	void Render();

	// Called after the engine renders
	void PostRender();

	// Log a message for debug use
	void LogMessage();

	//GameInterface gameInterface;

};

#endif

