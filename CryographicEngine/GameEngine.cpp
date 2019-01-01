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
	window = new Window("Cryographic Engine", 1280, 720);
	renderer = new OpenGLRenderer(window);
	sceneGraph = new SceneGraph();
	frustum = new Frustum();
	camera = new Camera();
	//frustum->WindowResize(camera->GetFOV(), static_cast<float>(window->GetWidth() / window->GetHeight()), 0.1f, 100.0f);
	//frustum->CameraMovement(camera->GetPosition(), -camera->GetFront(), camera->GetUp(), camera->GetRight());
	
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
	ShaderManager::GetInstance()->StoreShader(std::string("shadowDbug"), "../Shaders/shadowDebug.vs", "../Shaders/shadowDebug.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("clay"), "../Shaders/clay.vs", "../Shaders/clay.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("terrain"), "../Shaders/terrain.vs", "../Shaders/terrain.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("plainColour"), "../Shaders/colourNoLight.vs", "../Shaders/colourNoLight.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("lowDetail"), "../Shaders/lowDetailModel.vs", "../Shaders/lowDetailModel.fs");

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
	Material *whiteMat = new Material(glm::vec3(1.0f, 1.0f, 1.0f), ShaderManager::GetInstance()->GetShader(std::string("plainColour")));
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
	Mesh *woodBox = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("woodPlank")));
	Mesh *reflectiveBox = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("reflective")));
	Mesh *whiteBox = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("white")));
	Mesh *brickBox = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("brick")));
	brickBox->SetTextureScale(7.0f, 7.0f);
	woodBox->SetTextureScale(10.0f, 10.0f);
	MeshManager::GetInstance()->StoreMesh(std::string("woodBox"), woodBox);
	MeshManager::GetInstance()->StoreMesh(std::string("reflectiveBox"), reflectiveBox);
	MeshManager::GetInstance()->StoreMesh(std::string("whiteBox"), whiteBox);
	MeshManager::GetInstance()->StoreMesh(std::string("brickBox"), brickBox);

	// Models
	Model *nanosuitModel = ModelManager::GetInstance()->LoadModel(std::string("nanosuit"), std::string("../Resources/nanosuit/nanosuit.obj"));
	nanosuitModel->SetShininess(256.0f);
	nanosuitModel->SetReflectiveness(1.0f);

	Model *cyborgModel = ModelManager::GetInstance()->LoadModel(std::string("cyborg"), std::string("../Resources/cyborg/cyborg.obj"), ShaderManager::GetInstance()->GetShader(std::string("defaultModelNormals")));
	cyborgModel->SetReflectiveness(1.0f);

	Model *cyborgModel01 = ModelManager::GetInstance()->LoadModel(std::string("cyborg01"), std::string("../Resources/cyborg01/cyborg01.obj"), ShaderManager::GetInstance()->GetShader(std::string("defaultModelNormals")));
	cyborgModel01->SetReflectiveness(1.0f);

	// LOD grouping
	LevelOfDetail *cyborgLOD = new LevelOfDetail(cyborgModel);
	cyborgLOD->AddNextLOD(cyborgModel01, 30.0f);

	// Lights
	pointLight = new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.014f, 0.0007f);
	spotLight = new SpotLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.2f, -1.0f), 0.014f, 0.0007f, 10.0f, 12.5f);
	dirLight = new DirectionalLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f, -0.6f, 0.2f));

	// GameObjects
	GameObject *empty = new GameObject(std::string("emptyGameObject"));

	GameObject *dirgameObject = new GameObject(std::string("dirLight"), dirLight);

	GameObject *spotgameObject = new GameObject(std::string("spotLight1"), spotLight);

	GameObject *pointGameObject = new GameObject(std::string("pointLight1"), pointLight);
	pointGameObject->SetPosition(glm::vec3(0.0f, 4.0f, 0.0f));

	GameObject *cyborg = new GameObject(std::string("cyborg"), cyborgLOD);
	cyborg->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cyborg->SetScale(glm::vec3(1.0f));
	cyborg->HighQualityShadows(false);

	GameObject *OrbitingLight = new GameObject(std::string("test"), MeshManager::GetInstance()->GetMesh(std::string("whiteBox")));
	OrbitingLight->SetScale(glm::vec3(1.0f));
	OrbitingLight->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	OrbitingLight->SetCastShadows(false);

	GameObject *nanosuit = new GameObject(std::string("nanosuit"), nanosuitModel);
	nanosuit->SetScale(glm::vec3(0.25f));
	nanosuit->SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));

	GameObject *floor = new GameObject(std::string("floor"), MeshManager::GetInstance()->GetMesh(std::string("woodBox")));
	floor->SetScale(glm::vec3(100.0f, 1.0f, 100.0f));
	floor->SetPosition(glm::vec3(0.0f, -0.5f, 0.0f));


	// Scene Graph usage
	GetRootSceneNode()->AttachChild(empty->GetSceneNode());
    empty->AttachChild(dirgameObject);
	empty->AttachChild(cyborg);
	empty->AttachChild(pointGameObject);
	empty->AttachChild(spotgameObject);
	empty->AttachChild(nanosuit);
	empty->AttachChild(floor);
	pointGameObject->AttachChild(OrbitingLight);

	camera->SetPosition(glm::vec3(0.0f, 2.5f, 5.0f));

	Timer::GetInstance().Start();

	// Orbiting point light variables
	float radius = 25.0f;
	float angle = 0.0f;
	float speed = 0.35f;

	float timers = 0.0f;
	int loops = 0;
	
	while (isRunning) {

		Timer::GetInstance().Update();

		// Circle for the point light
		pointGameObject->SetPosition(glm::vec3(2.5f + (radius * glm::cos(angle)), pointGameObject->GetPosition().y, -2.0f + (radius * glm::sin(angle))));

		float deltaTime = Timer::GetInstance().GetDeltaTime();

		angle += speed * deltaTime;

		// Spot light follows the camera
		spotgameObject->SetPosition(camera->GetPosition());
		spotLight->SetDirection(camera->GetFront());

		timers += deltaTime;
		loops += 1;

		if (timers >= 1.0f) {
			std::cout << 1.0f / deltaTime << std::endl;
			timers = 0.0f;
			loops = 0;
		}

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
	renderer->Render(window, *frustum, camera, skybox, sceneGraph);
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
				// Turn off/on spot light with up arrow
			case SDLK_UP:
				if (spotLight->GetDiffuseColour() == glm::vec3(0.0f))
					spotLight->SetColour(glm::vec3(0.75f));
				else if (spotLight->GetDiffuseColour() == glm::vec3(0.75f))
					spotLight->SetColour(glm::vec3(0.0f));
				break;
				// Turn off/on directional light with down arrow
			case SDLK_DOWN:
				if (dirLight->GetDiffuseColour() == glm::vec3(0.0f))
					dirLight->SetColour(glm::vec3(0.75f));
				else if (dirLight->GetDiffuseColour() == glm::vec3(0.75f))
					dirLight->SetColour(glm::vec3(0.0f));
				break;
				// Turn off/on point light with spacebar
			case SDLK_SPACE:
				if (pointLight->GetDiffuseColour() == glm::vec3(0.0f))
					pointLight->SetColour(glm::vec3(1.0f));
				else if (pointLight->GetDiffuseColour() == glm::vec3(1.0f))
					pointLight->SetColour(glm::vec3(0.0f));
				break;
			case SDLK_F11:
				window->ToggleFullScreen();
				renderer->InitFrameBuffers(window);
				break;
			case SDLK_g:
				renderer->ToggleMSAA(window);
				break;
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
		}
		if (state[SDL_SCANCODE_DOWN]) {
		}
		if (state[SDL_SCANCODE_LEFT]) {
		}
		if (state[SDL_SCANCODE_RIGHT]) {
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