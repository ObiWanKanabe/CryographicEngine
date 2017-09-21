#include "GameEngine.h"

GameEngine* GameEngine::theInstance = nullptr;

Window* GameEngine::window = nullptr;

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
bool GameEngine::onStart() {
	window = new Window("DemoApp");
	return true;
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