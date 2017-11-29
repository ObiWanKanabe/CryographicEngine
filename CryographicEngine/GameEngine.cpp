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

GameEngine& GameEngine::GetInstance() {
	if (theInstance == nullptr) {
		theInstance = new GameEngine();
	}
	return *theInstance;
}

void GameEngine::OnStart() {
	window = new Window("DemoApp", 1200, 900);
	renderer = new OpenGLRenderer();
	triangle = new Triangle();	
	ShaderManager::GetInstance()->StoreShader(std::string("orangeish"), "../Shaders/vertexShaderSource.vs", "../Shaders/fragmentShaderSource.fs");
	ImageManager::GetInstance()->StoreImage(std::string("derp"), "../Resources/oreos.png");
	while (isRunning) {
		PreRender();
		Render();
		PostRender();
		SDL_Delay(1000.0f/FPS);
	}
	OnEnd();
}

void GameEngine::OnEnd() {
	renderer->~OpenGLRenderer();
	window->~Window();
}

void GameEngine::PreRender() {
	renderer->Clear();
}

void GameEngine::Render() {
	triangle->Render();
	renderer->RenderPrimitive(window);
	window->Clear();
}

void GameEngine::PostRender() {
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

void GameEngine::LogMessage() {

}