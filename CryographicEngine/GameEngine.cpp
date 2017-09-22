#include "GameEngine.h"

GameEngine* GameEngine::theInstance = nullptr;

Window* GameEngine::window = nullptr;

OpenGLRenderer* GameEngine::renderer = nullptr;

GameEngine::GameEngine() {

}

GameEngine::~GameEngine() {

}

GameEngine& GameEngine::getInstance() {
	if (theInstance == nullptr) {
		theInstance = new GameEngine();
	}
	return *theInstance;
}

void GameEngine::onStart() {
	window = new Window("DemoApp", 1200, 900);
	renderer = new OpenGLRenderer(window);
}

void GameEngine::onEnd() {

}

void GameEngine::preRender() {

}

void GameEngine::render() {

}

void GameEngine::postRender() {

}

void GameEngine::logMessage() {

}