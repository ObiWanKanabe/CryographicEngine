#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Window.h"
#include "OpenGLRenderer.h"
#include "GameInterface.h"
#include "Timer.h"
#include "SceneGraph.h"
#include "SceneNode.h"
#include "GameObject.h"

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

	// The scene graph of the engine
	SceneGraph *sceneGraph;

	// The frustum to be used when rendering objects on screen
	Frustum *frustum;

	// Testing in Engine
	Camera* camera;
	SDL_Event events;
	CubeMap* skybox;

	SpotLight* spotLight;
	DirectionalLight *dirLight;
	PointLight *pointLight;

	GameObject* nanosuit;

public:

	// Default Deconstructor
	~GameEngine();

	// Returns the single instance of the Engine
	static GameEngine* GetInstance();

	// Returns the Engine's window
	inline static Window& GetWindow() { return *window; };

	SceneNode* GetRootSceneNode();

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

	// Handle the input for the game
	void HandleInput();

	// Log a message for debug use
	void LogMessage();

	//GameInterface gameInterface;

};

#endif

