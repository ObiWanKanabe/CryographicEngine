#include "GameEngine.h"

GameEngine* GameEngine::theInstance = nullptr;
Window* GameEngine::window = nullptr;
OpenGLRenderer* GameEngine::renderer = nullptr;

GameEngine::GameEngine() {

}

GameEngine::~GameEngine() {
	delete window;
	delete renderer;
	delete sceneGraph;
	delete frustum;
	delete camera;
	delete theInstance;
	window = nullptr;
	renderer = nullptr;
	sceneGraph = nullptr;
	frustum = nullptr;
	camera = nullptr;
	theInstance = nullptr;
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
	frustum->WindowResize(camera->GetFOV(), static_cast<float>(window->GetWidth() / window->GetHeight()), 0.1f, 100.0f);
	frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
	
	//Shaders
	ShaderManager::GetInstance()->StoreShader(std::string("defaultImage"), "../Shaders/imgVertexShader.vs", "../Shaders/imgFragmentShader.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultImageNormals"), "../Shaders/defaultImageNormals.vs", "../Shaders/defaultImageNormals.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultColour"), "../Shaders/colourVertexShader.vs", "../Shaders/colourFragmentShader.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultSkybox"), "../Shaders/cubemapVertexShader.vs", "../Shaders/cubemapFragmentShader.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultReflective"), "../Shaders/reflectVertexShader.vs", "../Shaders/reflectFragmentShader.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultRefractive"), "../Shaders/refract.vs", "../Shaders/refract.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultModel"), "../Shaders/model.vs", "../Shaders/model.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultModelNormals"), "../Shaders/modelNormals.vs", "../Shaders/modelNormals.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultShadowDepth"), "../Shaders/defaultShadow.vs", "../Shaders/defaultShadow.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("clay"), "../Shaders/clay.vs", "../Shaders/clay.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("terrain"), "../Shaders/terrain.vs", "../Shaders/terrain.fs");

	// Skybox Images

	// Tutorial skybox
	//ImageManager::GetInstance()->StoreImage(std::string("right"), "../Resources/right.jpg");
	//ImageManager::GetInstance()->StoreImage(std::string("left"), "../Resources/left.jpg");
	//ImageManager::GetInstance()->StoreImage(std::string("top"), "../Resources/top.jpg");
	//ImageManager::GetInstance()->StoreImage(std::string("bottom"), "../Resources/bottom.jpg");
	//ImageManager::GetInstance()->StoreImage(std::string("back"), "../Resources/back.jpg");
	//ImageManager::GetInstance()->StoreImage(std::string("front"), "../Resources/front.jpg");

	// Scott's skybox
	//ImageManager::GetInstance()->StoreImage(std::string("right"), "../Resources/posx.jpg");
	//ImageManager::GetInstance()->StoreImage(std::string("left"), "../Resources/negx.jpg");
	//ImageManager::GetInstance()->StoreImage(std::string("top"), "../Resources/posy.jpg");
	//ImageManager::GetInstance()->StoreImage(std::string("bottom"), "../Resources/negy.jpg");
	//ImageManager::GetInstance()->StoreImage(std::string("back"), "../Resources/posz.jpg");
	//ImageManager::GetInstance()->StoreImage(std::string("front"), "../Resources/negz.jpg");

	// Nebula generated skybox
	ImageManager::GetInstance()->StoreImage(std::string("right"), "../Resources/BlueStars/right.png");
	ImageManager::GetInstance()->StoreImage(std::string("left"), "../Resources/BlueStars/left.png");
	ImageManager::GetInstance()->StoreImage(std::string("top"), "../Resources/BlueStars/top.png");
	ImageManager::GetInstance()->StoreImage(std::string("bottom"), "../Resources/BlueStars/bottom.png");
	ImageManager::GetInstance()->StoreImage(std::string("back"), "../Resources/BlueStars/back.png");
	ImageManager::GetInstance()->StoreImage(std::string("front"), "../Resources/BlueStars/front.png");

	std::vector<Image*> skyboxImageList = {
	ImageManager::GetInstance()->GetImage(std::string("right")),
	ImageManager::GetInstance()->GetImage(std::string("left")),
	ImageManager::GetInstance()->GetImage(std::string("top")),
	ImageManager::GetInstance()->GetImage(std::string("bottom")),
	ImageManager::GetInstance()->GetImage(std::string("back")),
	ImageManager::GetInstance()->GetImage(std::string("front")),
	};
	
	// Images
	ImageManager::GetInstance()->StoreImage(std::string("wood_diff"), "../Resources/woodbox_diffuse.png");
	ImageManager::GetInstance()->GetImage(std::string("wood_diff"))->SetMapType(MAP_TYPE::DIFFUSE);

	ImageManager::GetInstance()->StoreImage(std::string("wood_spec"), "../Resources/woodbox_specular.png");
	ImageManager::GetInstance()->GetImage(std::string("wood_spec"))->SetMapType(MAP_TYPE::SPECULAR);

	ImageManager::GetInstance()->StoreImage(std::string("wood"), "../Resources/wood.png");

	ImageManager::GetInstance()->StoreImage(std::string("brickwall"), "../Resources/brickwall.jpg");
	ImageManager::GetInstance()->GetImage(std::string("brickwall"))->SetMapType(MAP_TYPE::DIFFUSE);

	ImageManager::GetInstance()->StoreImage(std::string("brickwall_norm"), "../Resources/brickwall_normal.jpg");
	ImageManager::GetInstance()->GetImage(std::string("brickwall_norm"))->SetMapType(MAP_TYPE::NORMAL);


	// Materials
	Material *woodBoxMat = new Material(ImageManager::GetInstance()->GetImage(std::string("wood_diff")), ShaderManager::GetInstance()->GetShader(std::string("defaultImage")));
	woodBoxMat->AddImage(ImageManager::GetInstance()->GetImage(std::string("wood_spec")));
	Material *reflectiveMat = new Material(MATERIAL_TYPE::REFLECTIVE, ShaderManager::GetInstance()->GetShader(std::string("defaultReflective")));
	Material *whiteMat = new Material(glm::vec3(1.0f, 1.0f, 1.0f), ShaderManager::GetInstance()->GetShader(std::string("defaultColour")));
	Material *woodPlankMat = new Material(ImageManager::GetInstance()->GetImage(std::string("wood")));
	Material *brickMat = new Material(ImageManager::GetInstance()->GetImage(std::string("brickwall")));
	brickMat->AddImage(ImageManager::GetInstance()->GetImage(std::string("brickwall_norm")));
	MaterialManager::GetInstance()->StoreMaterial(std::string("woodBox"), woodBoxMat);
	MaterialManager::GetInstance()->StoreMaterial(std::string("reflective"), reflectiveMat);
	MaterialManager::GetInstance()->StoreMaterial(std::string("white"), whiteMat);
	MaterialManager::GetInstance()->StoreMaterial(std::string("woodPlank"), woodPlankMat);
	MaterialManager::GetInstance()->StoreMaterial(std::string("brick"), brickMat);

	// Skybox
	skybox = new CubeMap(skyboxImageList);

	// Meshes
	Mesh *woodBox = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("woodBox")));
	Mesh *reflectiveBox = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("reflective")));
	Mesh *whiteBox = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("white")));
	Mesh *brickBox = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("brick")));
	brickBox->SetTextureScale(7.0f, 7.0f);
	MeshManager::GetInstance()->StoreMesh(std::string("woodbox"), woodBox);
	MeshManager::GetInstance()->StoreMesh(std::string("reflectiveBox"), reflectiveBox);
	MeshManager::GetInstance()->StoreMesh(std::string("whiteBox"), whiteBox);
	MeshManager::GetInstance()->StoreMesh(std::string("brickBox"), brickBox);

	// Models
	Model *nanosuitModel = new Model(std::string("../Resources/nanosuit/nanosuit.obj"), ShaderManager::GetInstance()->GetShader(std::string("defaultModel")));
	ModelManager::GetInstance()->StoreModel(std::string("nanosuit"), nanosuitModel);
	nanosuitModel->SetShininess(256.0f);
	nanosuitModel->SetReflectiveness(1.0f);

	Model *cyborgModel = new Model(std::string("../Resources/cyborg/cyborg.obj"), ShaderManager::GetInstance()->GetShader(std::string("defaultModelNormals")));
	ModelManager::GetInstance()->StoreModel(std::string("cyborg"), cyborgModel);
	cyborgModel->SetReflectiveness(0.75f);

	Model *gameLoftModel = new Model(std::string("../Resources/clay/gameloftLogo.obj"), ShaderManager::GetInstance()->GetShader(std::string("clay")));
	ModelManager::GetInstance()->StoreModel(std::string("clay"), gameLoftModel);
	gameLoftModel->SetShininess(256.0f);

	Model *terrainModel = new Model(std::string("../Resources/terrain/terrain.obj"), ShaderManager::GetInstance()->GetShader(std::string("terrain")));
	ModelManager::GetInstance()->StoreModel(std::string("terrain"), terrainModel);
	terrainModel->SetBackFaceCulling(false);

	// Lights
	spotLight1 = new Light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.2f, -1.0f), 0.09f, 0.032f, 10.0f, 12.5f);
	Light *dirLight = new Light(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.8f, -0.4f, -1.0f));

	// GameObjects
	GameObject *empty = new GameObject(std::string("emptyGameObject"));

	GameObject *dirgameObject = new GameObject(std::string("dirLight"), dirLight);

	GameObject *spotGameObject = new GameObject(std::string("spotLight1"), spotLight1);
	spotGameObject->SetPosition(glm::vec3(0.0f, 4.25f, 5.0f));

	GameObject *clay = new GameObject(std::string("clay"), ModelManager::GetInstance()->GetModel(std::string("clay")));
	clay->SetScale(glm::vec3(0.02f));
	clay->SetPosition(glm::vec3(0.0f, 3.0f, -2.0f));
	clay->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));

	GameObject *terrain = new GameObject(std::string("terrain"), ModelManager::GetInstance()->GetModel(std::string("terrain")));
	terrain->SetScale(glm::vec3(0.02f));
	terrain->SetPosition(glm::vec3(5.0f, 0.0f, -2.0f));

	GameObject *nanosuit = new GameObject(std::string("nanosuit"), ModelManager::GetInstance()->GetModel(std::string("cyborg")));
	nanosuit->SetPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
	nanosuit->SetScale(glm::vec3(1.0f));

	// Scene Graph usage
	GetRootSceneNode()->AttachChild(empty->GetSceneNode());
	empty->AttachChild(terrain);
	empty->AttachChild(clay);
	empty->AttachChild(dirgameObject);
	empty->AttachChild(nanosuit);
	clay->AttachChild(spotGameObject);

	camera->SetPosition(glm::vec3(0.0f, 2.5f, 5.0f));

	Timer::GetInstance().Start();
	
	while (isRunning) {

		Timer::GetInstance().Update();

		PreRender();
		Render();
		PostRender();
	}
	OnEnd();
}

void GameEngine::OnEnd() {
	renderer->~OpenGLRenderer();
	window->~Window();
	sceneGraph->~SceneGraph();
	frustum->~Frustum();
	camera->~Camera();
}

void GameEngine::PreRender() {
	Timer::GetInstance().Update();
	window->Clear();
	renderer->Clear();
	renderer->PreRender(window, camera, skybox);
	HandleInput();
}

void GameEngine::Render() {
	sceneGraph->RenderSceneGraph(*frustum, *renderer, camera, skybox);
	//sceneGraph->Render(*frustum, *renderer, camera, skybox);
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
			//frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
		}
		if (state[SDL_SCANCODE_A]) {
			camera->ProcessKeyboard(CAMERA_DIRECTION::LEFT, 0.01f);
			//frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
		}
		if (state[SDL_SCANCODE_S]) {
			camera->ProcessKeyboard(CAMERA_DIRECTION::BACKWARD, 0.01f);
			//frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
		}
		if (state[SDL_SCANCODE_D]) {
			camera->ProcessKeyboard(CAMERA_DIRECTION::RIGHT, 0.01f);
			//frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
		}

		if (state[SDL_SCANCODE_UP]) {
			spotLight1->SetDirection(glm::vec3(spotLight1->GetDirection().x, spotLight1->GetDirection().y + 0.01f, spotLight1->GetDirection().z));
		}
		if (state[SDL_SCANCODE_DOWN]) {
			spotLight1->SetDirection(glm::vec3(spotLight1->GetDirection().x, spotLight1->GetDirection().y - 0.01f, spotLight1->GetDirection().z));
		}
		if (state[SDL_SCANCODE_LEFT]) {
			spotLight1->SetDirection(glm::vec3(spotLight1->GetDirection().x - 0.01f, spotLight1->GetDirection().y, spotLight1->GetDirection().z));
		}
		if (state[SDL_SCANCODE_RIGHT]) {
			spotLight1->SetDirection(glm::vec3(spotLight1->GetDirection().x + 0.01f, spotLight1->GetDirection().y, spotLight1->GetDirection().z));
		}

		if (events.type == SDL_MOUSEMOTION) {
			camera->ProcessMouseMovement((float)events.motion.x, (float)events.motion.y);
			//frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
		}

		if (events.type == SDL_MOUSEWHEEL) {
			camera->ProcessMouseScroll((float)events.wheel.y);
			//frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
		}
	}
}

void GameEngine::LogMessage() {

}