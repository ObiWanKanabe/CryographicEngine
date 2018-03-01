#include "Model.h"

Model::Model(std::string filePath) {
	LoadModel(filePath);
}

void Model::BindUniforms(Camera *camera, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->BindUniforms(camera, modelMatrix, viewMatrix, projectionMatrix);
	}
}

void Model::PreRender() {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->PreRender();
	}
}

void Model::Render() {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->Render();
	}
}

void Model::PostRender() {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->PostRender();
	}
}

void Model::LoadModel(std::string filePath) {
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

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

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene *scene) {
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	float offset = 0;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// process vertex positions, normals and texture coordinates
		vertices[i + offset] = mesh->mVertices[i].x;
		vertices[i + 1 + offset] = mesh->mVertices[i].y;
		vertices[i + 2 + offset] = mesh->mVertices[i].z;

		vertices[i + 3 + offset] = mesh->mNormals[i].x;
		vertices[i + 4 + offset] = mesh->mNormals[i].y;
		vertices[i + 5 + offset] = mesh->mNormals[i].z;
		if (mesh->mTextureCoords[0]) {
			vertices[i + 6 + offset] = mesh->mTextureCoords[0][i].x;
			vertices[i + 7 + offset] = mesh->mTextureCoords[0][i].y;
		}
		else {
			vertices[i + 6 + offset] = 0.0f;
			vertices[i + 7 + offset] = 0.0f;
		}
		offset += 8;
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
		// process material
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Texture> diffuseMaps = LoadMaterialTextures(material,
				aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<Texture> specularMaps = LoadMaterialTextures(material,
				aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

	return &Mesh(vertices, indices, textures);
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
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;

			// Loading Model Texture
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
			loadedTextures.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}