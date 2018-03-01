#include "GameEngine.h"

GameEngine* GameEngine::theInstance = nullptr;
Window* GameEngine::window = nullptr;
OpenGLRenderer* GameEngine::renderer = nullptr;

GameEngine::GameEngine() {

}

GameEngine::~GameEngine() {
	delete window;
	delete renderer;
	window = nullptr;
	renderer = nullptr;
}

GameEngine* GameEngine::GetInstance() {
	if (theInstance == nullptr) {
		theInstance = new GameEngine();
	}
	return theInstance;
}

SceneNode* GameEngine::GetRootSceneNode() {
	return sceneGraph->GetRootSceneNode();
}

void GameEngine::OnStart() {
	window = new Window("Cryographic Engine", 1200, 900);
	renderer = new OpenGLRenderer();
	sceneGraph = new SceneGraph();
	frustum = new Frustum();
	camera = new Camera();
	frustum->WindowResize(camera->GetFOV(), 1200.0f / 900.0f, 0.1f, 100.0f);
	frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
	
	//Shaders
	ShaderManager::GetInstance()->StoreShader(std::string("defaultImage"), "../Shaders/imgVertexShader.vs", "../Shaders/imgFragmentShader.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultColour"), "../Shaders/colourVertexShader.vs", "../Shaders/colourFragmentShader.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultSkybox"), "../Shaders/cubemapVertexShader.vs", "../Shaders/cubemapFragmentShader.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultReflective"), "../Shaders/reflectVertexShader.vs", "../Shaders/reflectFragmentShader.fs");

	// Skybox Images
	ImageManager::GetInstance()->StoreImage(std::string("right"), "../Resources/right.jpg");
	ImageManager::GetInstance()->StoreImage(std::string("left"), "../Resources/left.jpg");
	ImageManager::GetInstance()->StoreImage(std::string("top"), "../Resources/top.jpg");
	ImageManager::GetInstance()->StoreImage(std::string("bottom"), "../Resources/bottom.jpg");
	ImageManager::GetInstance()->StoreImage(std::string("back"), "../Resources/back.jpg");
	ImageManager::GetInstance()->StoreImage(std::string("front"), "../Resources/front.jpg");

	ImageManager::GetInstance()->StoreImage(std::string("right"), "../Resources/posx.jpg");
	ImageManager::GetInstance()->StoreImage(std::string("left"), "../Resources/negx.jpg");
	ImageManager::GetInstance()->StoreImage(std::string("top"), "../Resources/posy.jpg");
	ImageManager::GetInstance()->StoreImage(std::string("bottom"), "../Resources/negy.jpg");
	ImageManager::GetInstance()->StoreImage(std::string("back"), "../Resources/posz.jpg");
	ImageManager::GetInstance()->StoreImage(std::string("front"), "../Resources/negz.jpg");

	std::vector<Image*> skyboxImageList = {
	ImageManager::GetInstance()->GetImage(std::string("right")),
	ImageManager::GetInstance()->GetImage(std::string("left")),
	ImageManager::GetInstance()->GetImage(std::string("top")),
	ImageManager::GetInstance()->GetImage(std::string("bottom")),
	ImageManager::GetInstance()->GetImage(std::string("back")),
	ImageManager::GetInstance()->GetImage(std::string("front")),
	};
	
	// Images
	ImageManager::GetInstance()->StoreImage(std::string("wack"), "../Resources/wack.jpg");

	// Materials
	Material *myMaterial = new Material(ImageManager::GetInstance()->GetImage(std::string("wack")), ShaderManager::GetInstance()->GetShader(std::string("defaultImage")));
	Material *myMaterial2 = new Material(MATERIAL_TYPE::REFLECTIVE, ShaderManager::GetInstance()->GetShader(std::string("defaultReflective")));
	Material *myMaterial3 = new Material(glm::vec3(0.0f, 1.0f, 1.0f), ShaderManager::GetInstance()->GetShader(std::string("defaultColour")));
	MaterialManager::GetInstance()->StoreMaterial(std::string("material"), myMaterial);
	MaterialManager::GetInstance()->StoreMaterial(std::string("material2"), myMaterial2);
	MaterialManager::GetInstance()->StoreMaterial(std::string("material3"), myMaterial3);

	// Skybox
	skybox = new CubeMap(skyboxImageList);

	// Meshes
	Mesh *mesh = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("material")));
	Mesh *mesh2 = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("material2")));
	Mesh *mesh3 = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("material3")));
	MeshManager::GetInstance()->StoreMesh(std::string("mesh"), mesh);
	MeshManager::GetInstance()->StoreMesh(std::string("mesh2"), mesh2);
	MeshManager::GetInstance()->StoreMesh(std::string("mesh3"), mesh3);

	wackBox = new GameObject(std::string("derp"), MeshManager::GetInstance()->GetMesh(std::string("mesh")));
	wackBox->SetPosition(glm::vec3(3.0f, 0.0f, -7.0f));
	wackBox->SetRotation(glm::vec3(0.0f, 0.0f, 15.0f));
	wackBox->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
	GetRootSceneNode()->AttachChild(wackBox->GetSceneNode());

	reflectiveBox = new GameObject(std::string("reflect"), MeshManager::GetInstance()->GetMesh(std::string("mesh2")));
	reflectiveBox->SetPosition(glm::vec3(1.7f, -0.3f, -2.3f));
	reflectiveBox->SetRotation(glm::vec3(90.0f, 0.0f, -25.0f));
	reflectiveBox->SetScale(glm::vec3(0.05f, 1.0f, 1.0f));

	wackBox->AttachChild(reflectiveBox);

	tealBox = new GameObject(std::string("teal"), MeshManager::GetInstance()->GetMesh(std::string("mesh2")));
	tealBox->SetPosition(glm::vec3(-2.0f, 1.0f, -2.5f));
	tealBox->SetRotation(glm::vec3(35.0f, 35.0f, 0.0f));

	reflectiveBox->AttachChild(tealBox);

	Timer::GetInstance().Start();

	while (isRunning) {
		//reflectiveBox->SetRotation(glm::quat(reflectiveBox->GetRotation().w + 0.001f, glm::vec3(0.0f, 1.0f, 0.0f)));
		wackBox->GetBoundingVolume()->SetPosition(wackBox->GetWorldPosition());
		tealBox->GetBoundingVolume()->SetPosition(tealBox->GetWorldPosition());
		reflectiveBox->GetBoundingVolume()->SetPosition(reflectiveBox->GetWorldPosition());
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
	Timer::GetInstance().Update();
	window->Clear();
	renderer->Clear();
	renderer->PreRender(window, camera, skybox);
	HandleInput();
}

void GameEngine::Render() {
	//renderer->Render(window, camera, skybox);
	sceneGraph->RenderSceneGraph(*frustum, *renderer, camera, skybox);
	renderer->Render(window, camera, skybox);
}

void GameEngine::PostRender() {
	renderer->PostRender(window, camera, skybox);
}

void GameEngine::HandleInput() {
	while (SDL_PollEvent(&events)) {

		const Uint8 *state = SDL_GetKeyboardState(NULL);

		switch (events.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			switch (events.key.keysym.sym) {
			case SDLK_ESCAPE:
				isRunning = false;
				break;
			default:
				break;
			}
		}
		if (state[SDL_SCANCODE_W]) {
			camera->ProcessKeyboard(CAMERA_DIRECTION::FORWARD, 0.01f);
			frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
		}
		if (state[SDL_SCANCODE_A]) {
			camera->ProcessKeyboard(CAMERA_DIRECTION::LEFT, 0.01f);
			frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
		}
		if (state[SDL_SCANCODE_S]) {
			camera->ProcessKeyboard(CAMERA_DIRECTION::BACKWARD, 0.01f);
			frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
		}
		if (state[SDL_SCANCODE_D]) {
			camera->ProcessKeyboard(CAMERA_DIRECTION::RIGHT, 0.01f);
			frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
		}

		if (events.type == SDL_MOUSEMOTION) {
			camera->ProcessMouseMovement((float)events.motion.x, (float)events.motion.y);
			frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
		}

		if (events.type == SDL_MOUSEWHEEL) {
			camera->ProcessMouseScroll((float)events.wheel.y);
			frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
		}
	}
}

void GameEngine::LogMessage() {

}