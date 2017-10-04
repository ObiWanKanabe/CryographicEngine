#include "GameEngine.h"

GameEngine* GameEngine::theInstance = nullptr;
Window* GameEngine::window = nullptr;
OpenGLRenderer* GameEngine::renderer = nullptr;

GameEngine::GameEngine() {

}

GameEngine::~GameEngine() {
	theInstance = nullptr;
	window = nullptr;
	renderer = nullptr;
	delete theInstance;
	delete window;
	delete renderer;
}

GameEngine& GameEngine::getInstance() {
	if (theInstance == nullptr) {
		theInstance = new GameEngine();
	}
	return *theInstance;
}

void GameEngine::onStart() {
	window = new Window("DemoApp", 1200, 900);
	renderer = new OpenGLRenderer();
	while (isRunning) {
		preRender();
		render();
		postRender();
		SDL_Delay(1000.0f/FPS);
	}
	onEnd();
}

void GameEngine::onEnd() {
	renderer->~OpenGLRenderer();
	window->~Window();
}

void GameEngine::preRender() {
	renderer->clear();
}

void GameEngine::render() {
	renderer->renderPrimitive(window);
	window->clear();
}

void GameEngine::postRender() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				isRunning = false;
				break;
			default:
				break;
			}
		}
	}
}

void GameEngine::logMessage() {

}