#include "Model.h"

Model::Model(std::string filePath) {
	LoadModel(filePath);
	shaderName = std::string("defaultModel");
	isBackCulled = true;
}

Model::Model(std::string filePath, Shader* shader) {
	LoadModel(filePath);
	name = "";
	if (shader->GetName() == "" && name == "") {
		std::cerr << "Shader provided for model is not in the shader manager and cannot be used" << std::endl;
	}
	else if (shader->GetName() == "") {
		std::cerr << "Shader provided for: " << name << " is not in the shader manager and cannot be used" << std::endl;
	}
	shaderName = shader->GetName();
	isBackCulled = true;
}

void Model::SetName(std::string& _name) {
	name = _name;
}

void Model::SetShininess(float _shininess) {
	for (size_t i = 0; i < meshes.size(); i++) {
		meshes[i]->GetMaterial()->SetShininess(_shininess);
	}
}

void Model::SetReflectiveness(float _reflectiveness) {
	for (size_t i = 0; i < meshes.size(); i++) {
		meshes[i]->GetMaterial()->SetReflectiveness(_reflectiveness);
	}
}

std::string Model::GetName() {
	return name;
}

std::string Model::GetShaderName() {
	return shaderName;
}

std::vector<Mesh*> Model::GetMeshes() {
	return meshes;
}

void Model::PrintMeshNames() {
	for (size_t i = 0; i < meshes.size(); i++) {
		std::cout << meshes[i]->GetName() << std::endl;
	}
}

void Model::SetBackFaceCulling(bool _culled) {
	isBackCulled = _culled;
}

void Model::BindUniforms(Camera *camera, std::vector<Light*> lights, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	Shader* shader = ShaderManager::GetInstance()->GetShader(shaderName);
	shader->use();
	glm::mat4 model = glm::translate(modelMatrix, offset);
	glm::mat4 normal = glm::transpose(glm::inverse(model));
	int pointNr = 0;
	int spotNr = 0;
	for (size_t i = 0; i < lights.size(); i++) {
		lights[i]->BindUniforms(shader, pointNr, spotNr);
		if (lights[i]->GetType() == LIGHT_TYPE::POINT_LIGHT) {
			pointNr++;
		}
		else if (lights[i]->GetType() == LIGHT_TYPE::SPOT_LIGHT) {
			spotNr++;
		}
	}
	shader->SetMat4("model", model);
	shader->SetMat4("view", viewMatrix);
	shader->SetMat4("projection", projectionMatrix);
	shader->SetMat4("normalMatrix", normal);
	shader->SetVec3("cameraPos", camera->GetPosition());
}

void Model::PreRender() {
	if (!isBackCulled)
		glDisable(GL_CULL_FACE);

	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->PreRender();
	}
}

void Model::Render() {
	Shader* shader = ShaderManager::GetInstance()->GetShader(shaderName);
	shader->use();

	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->BindUniforms(shader);
		meshes[i]->Render();
	}

}

void Model::Draw() {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->Render();
	}
}

void Model::PostRender() {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->PostRender();
	}

	if (!isBackCulled)
		glEnable(GL_CULL_FACE);
}

void Model::LoadModel(std::string filePath) {
	// We're setting up the assimp scene here
	// Making sure the model uses only triangles and the UVs are flipped when necessary
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = filePath.substr(0, filePath.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
	
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* Model::ProcessMesh(aiMesh* _mesh, const aiScene *scene) {
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	glm::vec3 position;


	// Setting up the vertex data of the mesh
	for (unsigned int i = 0; i < _mesh->mNumVertices; i++)
	{
		// Position 
		vertices.push_back(_mesh->mVertices[i].x);
		vertices.push_back(_mesh->mVertices[i].y);
		vertices.push_back(_mesh->mVertices[i].z);

		// Storing all values of position
		position.x += _mesh->mVertices[i].x;
		position.y += _mesh->mVertices[i].y;
		position.z += _mesh->mVertices[i].z;

		// Normals
		vertices.push_back(_mesh->mNormals[i].x);
		vertices.push_back(_mesh->mNormals[i].y);
		vertices.push_back(_mesh->mNormals[i].z);

		// Texture Coordinates, if any
		if (_mesh->mTextureCoords[0]) {
			vertices.push_back(_mesh->mTextureCoords[0][i].x);
			vertices.push_back(_mesh->mTextureCoords[0][i].y);
		}
		else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}

		// Tangents and Bitangets, if any
		if (_mesh->HasTangentsAndBitangents()) {
			vertices.push_back(_mesh->mTangents[i].x);
			vertices.push_back(_mesh->mTangents[i].y);
			vertices.push_back(_mesh->mTangents[i].z);

			vertices.push_back(_mesh->mBitangents[i].x);
			vertices.push_back(_mesh->mBitangents[i].y);
			vertices.push_back(_mesh->mBitangents[i].z);
		}
		else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
	}

	// Setting up the face data of the mesh
	for (unsigned int i = 0; i < _mesh->mNumFaces; i++)
	{
		aiFace face = _mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Setting up the materials and textures of the mesh
	if (_mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[_mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		if (normalMaps.size() > 0) { 
			shaderName = "defaultModelNormals"; 
		}
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// Cheating here since assimp doesn't like reflection maps
		std::vector<Texture> reflectionMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_reflective");
		textures.insert(textures.end(), reflectionMaps.begin(), reflectionMaps.end());
	}

	// Calculating the position of the mesh from the origin
	position /= static_cast<float>(_mesh->mNumVertices);

	// Create the mesh using our mesh class and pass in the imported values
	Mesh *mesh = new Mesh(vertices, indices, textures, position, std::string(_mesh->mName.C_Str()), shaderName);
	return mesh;
}	

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < loadedTextures.size(); j++)
		{
			if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		// If texture hasn't been loaded already
		if (!skip)
		{  
			Texture texture;

			std::string filename = std::string(str.C_Str());
			filename = directory + '/' + filename;

			unsigned int textureID;
			glGenTextures(1, &textureID);

			Image *img = new Image(filename.c_str());

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, img->GetFormat(), img->GetWidth(), img->GetHeight(), 0, img->GetFormat(), GL_UNSIGNED_BYTE, img->GetData());
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
			texture.ID = textureID;
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
	}
	return textures;
}