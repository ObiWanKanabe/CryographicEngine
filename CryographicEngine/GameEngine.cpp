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
	ShaderManager::GetInstance()->StoreShader(std::string("defaultImageNormals"), "../Shaders/defaultImageNormals.vs", "../Shaders/defaultImageNormals.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultColour"), "../Shaders/colourVertexShader.vs", "../Shaders/colourFragmentShader.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultSkybox"), "../Shaders/cubemapVertexShader.vs", "../Shaders/cubemapFragmentShader.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultReflective"), "../Shaders/reflectVertexShader.vs", "../Shaders/reflectFragmentShader.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultRefractive"), "../Shaders/refract.vs", "../Shaders/refract.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultModel"), "../Shaders/model.vs", "../Shaders/model.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultModelNormals"), "../Shaders/modelNormals.vs", "../Shaders/modelNormals.fs");
	ShaderManager::GetInstance()->StoreShader(std::string("defaultShadowDepth"), "../Shaders/defaultShadow.vs", "../Shaders/defaultShadow.fs");

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
	ImageManager::GetInstance()->StoreImage(std::string("wack"), "../Resources/woodbox_diffuse.png");
	ImageManager::GetInstance()->GetImage(std::string("wack"))->SetMapType(MAP_TYPE::DIFFUSE);

	ImageManager::GetInstance()->StoreImage(std::string("wood_spec"), "../Resources/woodbox_specular.png");
	ImageManager::GetInstance()->GetImage(std::string("wood_spec"))->SetMapType(MAP_TYPE::SPECULAR);

	ImageManager::GetInstance()->StoreImage(std::string("wood"), "../Resources/wood.png");

	ImageManager::GetInstance()->StoreImage(std::string("brickwall"), "../Resources/brickwall.jpg");
	ImageManager::GetInstance()->GetImage(std::string("brickwall"))->SetMapType(MAP_TYPE::DIFFUSE);

	ImageManager::GetInstance()->StoreImage(std::string("brickwall_norm"), "../Resources/brickwall_normal.jpg");
	ImageManager::GetInstance()->GetImage(std::string("brickwall_norm"))->SetMapType(MAP_TYPE::NORMAL);


	// Materials
	Material *myMaterial = new Material(ImageManager::GetInstance()->GetImage(std::string("wack")), ShaderManager::GetInstance()->GetShader(std::string("defaultImage")));
	myMaterial->AddImage(ImageManager::GetInstance()->GetImage(std::string("wood_spec")));
	Material *myMaterial2 = new Material(MATERIAL_TYPE::REFLECTIVE, ShaderManager::GetInstance()->GetShader(std::string("defaultReflective")));
	Material *myMaterial3 = new Material(glm::vec3(1.0f, 1.0f, 1.0f), ShaderManager::GetInstance()->GetShader(std::string("defaultColour")));
	Material *myMaterial4 = new Material(ImageManager::GetInstance()->GetImage(std::string("wood")));
	Material *myMaterial5 = new Material(ImageManager::GetInstance()->GetImage(std::string("brickwall")));
	myMaterial5->AddImage(ImageManager::GetInstance()->GetImage(std::string("brickwall_norm")));
	MaterialManager::GetInstance()->StoreMaterial(std::string("material"), myMaterial);
	MaterialManager::GetInstance()->StoreMaterial(std::string("material2"), myMaterial2);
	MaterialManager::GetInstance()->StoreMaterial(std::string("material3"), myMaterial3);
	MaterialManager::GetInstance()->StoreMaterial(std::string("material4"), myMaterial4);
	MaterialManager::GetInstance()->StoreMaterial(std::string("material5"), myMaterial5);

	// Skybox
	skybox = new CubeMap(skyboxImageList);

	// Meshes
	Mesh *mesh = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("material")));
	Mesh *mesh2 = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("material2")));
	Mesh *mesh3 = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("material3")));
	Mesh *mesh4 = new Mesh(MESH_TYPE::CUBE, MaterialManager::GetInstance()->GetMaterial(std::string("material5")));
	mesh4->SetTextureScale(7.0f, 7.0f);
	MeshManager::GetInstance()->StoreMesh(std::string("mesh"), mesh);
	MeshManager::GetInstance()->StoreMesh(std::string("mesh2"), mesh2);
	MeshManager::GetInstance()->StoreMesh(std::string("mesh3"), mesh3);
	MeshManager::GetInstance()->StoreMesh(std::string("mesh4"), mesh4);

	// Models
	Model *model = new Model(std::string("../Resources/nanosuit/nanosuit.obj"), ShaderManager::GetInstance()->GetShader(std::string("defaultModelNormals")));
	ModelManager::GetInstance()->StoreModel(std::string("nanosuit"), model);
	model->SetShininess(256.0f);
	model->SetReflectiveness(1.0f);

	Model *model2 = new Model(std::string("../Resources/cyborg/cyborg.obj"), ShaderManager::GetInstance()->GetShader(std::string("defaultModelNormals")));
	ModelManager::GetInstance()->StoreModel(std::string("cyborg"), model2);
	

	// Lights
	Light *spotLight1 = new Light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.0f, -1.0f, -0.0f), 0.09f, 0.032f, 12.5f, 17.5f);
	Light *pointLight = new Light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, -0.0f), 0.09f, 0.032f, 12.5f, 17.5f);
	Light *dirLight = new Light(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.8f, 0.0f, -1.0f));

	//MaterialManager::GetInstance()->GetMaterial(MeshManager::GetInstance()->GetMesh(std::string("nanosuit_Body"))->GetMaterialName())->SetShaderName(std::string("defaultModel"));
	//MaterialManager::GetInstance()->GetMaterial(MeshManager::GetInstance()->GetMesh(std::string("nanosuit_Legs"))->GetMaterialName())->SetShaderName(std::string("defaultModel"));
	//MaterialManager::GetInstance()->GetMaterial(MeshManager::GetInstance()->GetMesh(std::string("nanosuit_Arms"))->GetMaterialName())->SetShaderName(std::string("defaultModel"));

	// GameObjects
	GameObject *dirgameObject = new GameObject(std::string("dirLight"), dirLight);

	GameObject *spotGameObject = new GameObject(std::string("spotLight"), spotLight1);

	GameObject *floor = new GameObject(std::string("derp"), MeshManager::GetInstance()->GetMesh(std::string("mesh4")));
	floor->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	floor->SetScale(glm::vec3(100.0f, 1.0f, 100.0f));

	box1 = new GameObject(std::string("derp"), MeshManager::GetInstance()->GetMesh(std::string("mesh")));
	box1->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	box2 = new GameObject(std::string("reflect"), MeshManager::GetInstance()->GetMesh(std::string("mesh")));
	box2->AttachLight(spotLight1);
	box2->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));

	box3 = new GameObject(std::string("teal"), MeshManager::GetInstance()->GetMesh(std::string("mesh")));
	box3->AttachLight(pointLight);
	box3->SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));

	nanosuitModel = new GameObject(std::string("crate1"), ModelManager::GetInstance()->GetModel(std::string("cyborg")));
	nanosuitModel->SetPosition(glm::vec3(15.0f, 0.0f, 0.0f));

	nanosuitHelmet = new GameObject(std::string("crate2"), ModelManager::GetInstance()->GetModel(std::string("nanosuit")));
	nanosuitHelmet->SetPosition(glm::vec3(20.0f, 0.0f, 0.0f));

	nanosuitVisor = new GameObject(std::string("crate3"), MeshManager::GetInstance()->GetMesh(std::string("nanosuit_Helmet")));
	nanosuitVisor->SetPosition(glm::vec3(25.0f, 0.0f, 0.0f));

	// Scene Graph usage
	GetRootSceneNode()->AttachChild(box1->GetSceneNode());
	//box1->AttachChild(floor);
	box1->AttachChild(spotGameObject);
	box1->AttachChild(box2);
	box1->AttachChild(nanosuitModel);
	box1->AttachChild(nanosuitHelmet);
	box1->AttachChild(nanosuitVisor);
	box1->AttachChild(box3);
	box1->AttachChild(dirgameObject);

	Timer::GetInstance().Start();
	
	while (isRunning) {
		PreRender();
		spotGameObject->SetPosition(camera->GetPosition());
		spotGameObject->GetAttachedLight()->SetDirection(camera->GetFront());
		Render();
		PostRender();
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
	//sceneGraph->RenderSceneGraph(*frustum, *renderer, camera, skybox);
	sceneGraph->Render(*frustum, *renderer, camera, skybox);
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
			box2->SetPosition(glm::vec3(box2->GetPosition().x, box2->GetPosition().y + 0.05f, box2->GetPosition().z));
		}
		if (state[SDL_SCANCODE_DOWN]) {
			box2->SetPosition(glm::vec3(box2->GetPosition().x, box2->GetPosition().y - 0.05f, box2->GetPosition().z));
		}
		if (state[SDL_SCANCODE_LEFT]) {
			box2->SetPosition(glm::vec3(box2->GetPosition().x - 0.05f, box2->GetPosition().y, box2->GetPosition().z));
		}
		if (state[SDL_SCANCODE_RIGHT]) {
			box2->SetPosition(glm::vec3(box2->GetPosition().x + 0.05f, box2->GetPosition().y, box2->GetPosition().z));
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