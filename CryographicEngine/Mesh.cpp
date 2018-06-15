#include "Mesh.h"

Mesh::Mesh(std::vector<GLfloat> _vertices, std::vector<unsigned int> _indices, std::vector <Texture> _textures, glm::vec3 _position, std::string &_name, std::string &_shaderName) {
	type = MESH_TYPE::MODEL;
	vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
	vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
	vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
	vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_TANGENT);
	vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_BITANGENT);
	vertices = _vertices;
	indices = _indices;
	textures = _textures;
	offset = -_position;
	name = _name;
	Material *material = new Material(_textures, _name);
	MaterialManager::GetInstance()->StoreMaterial(_name, material);
	materialName = material->GetName();
	material->SetShaderName(_shaderName);
	GenerateBuffers();
}

Mesh::Mesh(MESH_TYPE primType) {
	name = "";
	type = primType;
	offset = glm::vec3(0.0f);
	switch (primType) {
	case PLANE:
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
		vertices = {
			0.5f,  0.5f, 0.0f,  0.8f, 0.8f, 0.8f,
			-0.5f, 0.5f, 0.0f,  0.8f, 0.8f, 0.8f,
			-0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.8f,
			 0.5f, 0.5f, 0.0f,   0.8f, 0.8f, 0.8f,
			-0.5f,  -0.5f, 0.0f, 0.8f, 0.8f, 0.8f,
			0.5f,  -0.5f, 0.0f, 0.8f, 0.8f, 0.8f
		};
		break;
	case TRIANGLE:
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
		vertices = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.8f,
			0.5f, -0.5f, 0.0f,  0.8f, 0.8f, 0.8f,
			0.0f,  0.5f, 0.0f,  0.8f, 0.8f, 0.8f
		};
		break;
	case CUBE:
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
		vertices = {
			-0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,   0.8f, 0.8f, 0.8f,	0.0f,  0.0f, -1.0f,
			0.5f, -0.5f, -0.5f,   0.8f, 0.8f, 0.8f,	0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,   0.8f, 0.8f, 0.8f,	0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,	 0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.8f, 0.8f, 0.8f,	 0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	 0.0f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.8f, 0.8f, 0.8f,	 0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.8f, 0.8f, 0.8f,	 0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	-1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.8f, 0.8f, 0.8f,	-1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,	-1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,	-1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	-1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	-1.0f,  0.0f,  0.0f,

			0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,	1.0f,  0.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  0.8f, 0.8f, 0.8f,	1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,	1.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f, 0.8f, 0.8f, 0.8f,	 0.0f, -1.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,	0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, 0.8f, 0.8f, 0.8f,	 0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, 0.8f, 0.8f, 0.8f,	 0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f, 0.8f, 0.8f, 0.8f,	 0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	0.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  0.8f, 0.8f, 0.8f,	0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,	0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, 0.8f, 0.8f, 0.8f,	 0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, 0.8f, 0.8f, 0.8f,	 0.0f,  1.0f,  0.0f
		};
		break;
	default:
		std::cerr << "Not a valid prim type.\n";
		break;
	}
	GenerateBuffers();
}

Mesh::Mesh(MESH_TYPE primType, float r, float g, float b) {
	name = "";
	type = primType;
	offset = glm::vec3(0.0f);
	switch (primType) {
	case PLANE:
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
		vertices = {
			 0.5f, 0.5f, 0.0f,  r, g, b,
			-0.5f, 0.5f, 0.0f,  r, g, b,
			-0.5f, -0.5f, 0.0f, r, g, b,
			 0.5f, 0.5f, 0.0f,  r, g, b,
			-0.5f, -0.5f, 0.0f, r, g, b,
			 0.5f, -0.5f, 0.0f, r, g, b
		};
		break;
	case TRIANGLE:
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
		vertices = {
			-0.5f, -0.5f, 0.0f, r, g, b,
			0.5f, -0.5f, 0.0f,  r, g, b,
			0.0f,  0.5f, 0.0f,  r, g, b
		};
		break;
	case CUBE:
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
		vertices = {
			-0.5f, -0.5f, -0.5f,  r, g, b,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,   r, g, b, 0.0f,  0.0f, -1.0f,
			0.5f, -0.5f, -0.5f,   r, g, b, 0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,   r, g, b, 0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  r, g, b,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  r, g, b,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  r, g, b,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  r, g, b,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  r, g, b,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  r, g, b,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, r, g, b,   0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, r, g, b,   0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f,  r, g, b, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  r, g, b, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  r, g, b, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  r, g, b, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  r, g, b, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  r, g, b, -1.0f,  0.0f,  0.0f,

			0.5f,  0.5f,  0.5f,  r, g, b,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  r, g, b,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  r, g, b,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  r, g, b,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  r, g, b,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  r, g, b,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f, r, g, b,   0.0f, -1.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  r, g, b,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  r, g, b,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  r, g, b,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, r, g, b,   0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, r, g, b,   0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f, r, g, b,   0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  r, g, b,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  r, g, b,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  r, g, b,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, r, g, b,   0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, r, g, b,    0.0f,  1.0f,  0.0f
		};
		break;
	default:
		std::cerr << "Not a valid prim type.\n";
		break;
	}
	GenerateBuffers();
}

Mesh::Mesh(MESH_TYPE primType, Material* material) {
	name = "";
	if (material->GetName() == "" && name == "") {
		std::cerr << "Material provided for mesh is not in the material manager and cannot be used" << std::endl;
	}
	else if (material->GetName() == "") {
		std::cerr << "Material provides for mesh: " << name << " is not in the material manager and cannot be used" << std::endl;
	}
	materialName = material->GetName();
	type = primType;
	offset = glm::vec3(0.0f);
	if (material->GetType() == MATERIAL_TYPE::TEXTURE) {
		switch (primType) {
		case PLANE:
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
			vertices = {
				0.5f,  0.5f, 0.0f,   1.0f, 0.0f,  
				-0.5f, 0.5f, 0.0f,   0.0f, 0.0f,  
				-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 
				 0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 
				-0.5f,  -0.5f, 0.0f,  0.0f, 1.0f, 
				0.5f,  -0.5f, 0.0f,   1.0f, 1.0f   
			};

			break;
		case TRIANGLE:
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
			vertices = {
				-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 
				0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  
				0.0f,  0.5f, 0.0f,  0.5f, 0.0f  
			};
			break;
		case CUBE:
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);

			vertices = {
				-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
				0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
				0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
				0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
				-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,

				-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

				0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

				-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
				0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
			};

			break;
		default:
			std::cerr << "Not a valid prim type.\n";
			break;
		}
	}
	else if (material->GetType() == MATERIAL_TYPE::COLOUR) {
		switch (primType) {
		case PLANE:
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
			vertices = {
				0.5f,  0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f, 0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f, -0.5f, 0.0f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, 0.5f, 0.0f,   material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f,  -0.5f, 0.0f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f,  -0.5f, 0.0f, material->GetColour().x, material->GetColour().y, material->GetColour().z
			};
			break;
		case TRIANGLE:
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
			vertices = {
				-0.5f, -0.5f, 0.0f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.0f,  0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z
			};
			break;
		case CUBE:
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
			vertices = {
				-0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,  0.0f,  0.0f, -1.0f,
				0.5f,  0.5f, -0.5f,   material->GetColour().x, material->GetColour().y, material->GetColour().z, 0.0f,  0.0f, -1.0f,
				0.5f, -0.5f, -0.5f,   material->GetColour().x, material->GetColour().y, material->GetColour().z, 0.0f,  0.0f, -1.0f,
				0.5f,  0.5f, -0.5f,   material->GetColour().x, material->GetColour().y, material->GetColour().z, 0.0f,  0.0f, -1.0f,
				-0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,  0.0f,  0.0f, -1.0f,
				-0.5f,  0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,  0.0f,  0.0f, -1.0f,

				-0.5f, -0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,  0.0f,  0.0f, 1.0f,
				0.5f, -0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 0.0f,  0.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 0.0f,  0.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 0.0f,  0.0f, 1.0f,
				-0.5f,  0.5f,  0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,	  0.0f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,	  0.0f,  0.0f, 1.0f,

				-0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z, -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z, -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z, -1.0f,  0.0f,  0.0f,

				0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 1.0f,  0.0f,  0.0f,
				0.5f,  0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 1.0f,  0.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 1.0f,  0.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 1.0f,  0.0f,  0.0f,

				-0.5f, -0.5f, -0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,	  0.0f, -1.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 0.0f, -1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 0.0f, -1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f,  0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,	  0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f, -0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,	  0.0f, -1.0f,  0.0f,

				-0.5f,  0.5f, -0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,	  0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 0.0f,  1.0f,  0.0f,
				0.5f,  0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,	 0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f, -0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,	  0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f,  0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,	  0.0f,  1.0f,  0.0f
			};
			break;
		default:
			std::cerr << "Not a valid prim type.\n";
			break;
		}
	}
	else if (material->GetType() == MATERIAL_TYPE::REFLECTIVE) {
		switch (primType) {
		case PLANE:
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
			vertices = {
				0.5f,  0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f, 0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f, -0.5f, 0.0f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, 0.5f, 0.0f,   material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f,  -0.5f, 0.0f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f,  -0.5f, 0.0f, material->GetColour().x, material->GetColour().y, material->GetColour().z
			};
			break;
		case TRIANGLE:
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
			vertices = {
				-0.5f, -0.5f, 0.0f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.0f,  0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z
			};
			break;
		case CUBE:
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
			vertices = {
				-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
				0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
				0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
				0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
				0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
				0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

				-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

				0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
				0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
				0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f
			};
			break;
		default:
			std::cerr << "Not a valid prim type.\n";
			break;
		}
	}
	GenerateBuffers();
	material->Setup();
}
Mesh::~Mesh() {
	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);*/
}

void Mesh::SetName(std::string& _name) {
	name = _name;
}

void Mesh::SetShininess(float _shininess) {
	GetMaterial()->SetShininess(_shininess);
}

void Mesh::SetReflectiveness(float _reflectiveness) {
	GetMaterial()->SetReflectiveness(_reflectiveness);
}

void Mesh::SetTextureScale(float _x, float _y) {
	bool firstSet = true;
	for (int i = 0; i < vertexDescriptor.GetComponentList().size(); i++) {
		VertexComponentDescriptor vertexDesc = vertexDescriptor.GetComponentList()[i];
		if (vertexDesc.type == VertexComponentDescriptor::VERTEX_UV) {
			int offset = vertexDesc.offset / sizeof(float);
			int inc = vertexDescriptor.GetStride() / sizeof(float);
			int size = vertices.size() / inc;
			for (int j = 0; j < size; j++) {
				vertices[offset] *= _x;
				vertices[offset+1] *= _y;
				offset += inc;
			}
		}
	}
	GenerateBuffers();
}

std::string Mesh::GetName() {
	return name;
}

std::string Mesh::GetMaterialName() {
	return materialName;
}

Material* Mesh::GetMaterial() {
	Material *material = MaterialManager::GetInstance()->GetMaterial(materialName);
	return material;
}

void Mesh::AddComponent(VertexComponentDescriptor::VertexComponentType _type) {
	vertexDescriptor.AddComponent(_type);
}

void Mesh::GenerateBuffers() {
	if (type == MESH_TYPE::MODEL) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		for (int i = 0; i < vertexDescriptor.GetSize(); i++) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, vertexDescriptor.componentList[i].GetNumFloats(), GL_FLOAT, GL_FALSE, vertexDescriptor.GetStride(), (void*)(vertexDescriptor.componentList[i].offset));
		}

	}
	else {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

		for (int i = 0; i < vertexDescriptor.GetSize(); i++) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, vertexDescriptor.componentList[i].GetNumFloats(), GL_FLOAT, GL_FALSE, vertexDescriptor.GetStride(), (void*)(vertexDescriptor.componentList[i].offset));
		}
	}

	glBindVertexArray(0);
}

void Mesh::BindUniforms(Camera *camera, std::vector<Light*> lights, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{

	if (type == MESH_TYPE::MODEL) {
		if (materialName != "") {
			Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
			material->BindUniforms();
			glm::mat4 model = glm::translate(modelMatrix, offset);
			glm::mat4 normal = glm::transpose(glm::inverse(model));
			Shader* shader = material->GetShader();
			int pointNr = 0;
			int spotNr = 0;
			for (size_t i = 0; i < lights.size(); i++) {
				lights[i]->BindUniforms(shader, pointNr, spotNr);
			}
			shader->SetMat4("model", model);
			shader->SetMat4("view", viewMatrix);
			shader->SetMat4("projection", projectionMatrix);
			shader->SetMat4("normalMatrix", normal);
			shader->SetVec3("cameraPos", camera->GetPosition());
		}
		else {
			Shader *shader = ShaderManager::GetInstance()->GetShader(std::string("defaultModel"));
			shader->Use();
			shader->SetInt("texture_diffuse1", 0);
			shader->SetInt("texture_specular1", 0);
			shader->SetInt("texture_normal1", 0);
			glm::mat4 model = glm::translate(modelMatrix, offset);
			glm::mat4 normal = glm::transpose(glm::inverse(model));
			int pointNr = 0;
			int spotNr = 0;
			for (size_t i = 0; i < lights.size(); i++) {
				lights[i]->BindUniforms(shader, pointNr, spotNr);
			}
			shader->SetMat4("model", model);
			shader->SetMat4("view", viewMatrix);
			shader->SetMat4("projection", projectionMatrix);
			shader->SetMat4("normalMatrix", normal);
			shader->SetVec3("cameraPos", camera->GetPosition());
		}
	} else if (MaterialManager::GetInstance()->GetMaterial(materialName) != nullptr) {
		Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
		material->BindUniforms();
		Shader* shader = material->GetShader();
		glm::mat4 normal = glm::transpose(glm::inverse(modelMatrix));
		int pointNr = 0;
		int spotNr = 0;
		for (size_t i = 0; i < lights.size(); i++) {
			lights[i]->BindUniforms(shader, pointNr, spotNr);
		}
		shader->SetMat4("model", modelMatrix);
		shader->SetMat4("view", viewMatrix);
		shader->SetMat4("projection", projectionMatrix);
		shader->SetMat4("normalMatrix", normal);
		shader->SetVec3("cameraPos", camera->GetPosition());
	}
	else {
		Shader* shader = ShaderManager::GetInstance()->GetShader(std::string("defaultColour"));
		shader->Use();
		glm::mat4 normal = glm::transpose(glm::inverse(modelMatrix));
		int pointNr = 0;
		int spotNr = 0;
		for (size_t i = 0; i < lights.size(); i++) {
			lights[i]->BindUniforms(shader, pointNr, spotNr);
		}
		shader->SetMat4("model", modelMatrix);
		shader->SetMat4("view", viewMatrix);
		shader->SetMat4("projection", projectionMatrix);
		shader->SetMat4("normalMatrix", normal);
		shader->SetVec3("cameraPos", camera->GetPosition());
	}
}

void Mesh::BindUniformsLowDetail(Camera *camera, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	if (type == MESH_TYPE::MODEL) {
			Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
			Shader* shader = ShaderManager::GetInstance()->GetShader(std::string("lowDetail"));
			material->BindUniforms(shader);
			glm::mat4 model = glm::translate(modelMatrix, offset);
			shader->SetMat4("model", model);
			shader->SetMat4("view", viewMatrix);
			shader->SetMat4("projection", projectionMatrix);
			shader->SetVec3("cameraPos", camera->GetPosition());
	}
	else if (materialName != "") {
		Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
		Shader* shader;
		if (material->GetType() == MATERIAL_TYPE::COLOUR) {
			shader = ShaderManager::GetInstance()->GetShader(std::string("plainColour"));
		}
		else {
			shader = ShaderManager::GetInstance()->GetShader(std::string("lowDetail"));
		}
		material->BindUniforms(shader);
		shader->SetMat4("model", modelMatrix);
		shader->SetMat4("view", viewMatrix);
		shader->SetMat4("projection", projectionMatrix);
		shader->SetVec3("cameraPos", camera->GetPosition());
	}
	else {
		Shader* shader = ShaderManager::GetInstance()->GetShader(std::string("plainColour"));
		shader->Use();
		shader->SetMat4("model", modelMatrix);
		shader->SetMat4("view", viewMatrix);
		shader->SetMat4("projection", projectionMatrix);
	}
}

void Mesh::BindUniformsDepth(glm::mat4 modelMatrix, glm::mat4 lightSpaceMatrix) {
	Shader* shader = ShaderManager::GetInstance()->GetShader(std::string("defaultShadowDepth"));
	shader->Use();
	shader->SetMat4("model", modelMatrix);
	shader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
}

void Mesh::BindUniforms(Shader* shader) {
	if (materialName != "") {
		Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
		material->BindUniforms(shader);
	}
	else {
		shader->SetInt("material.diffuse1", 0);
		shader->SetInt("texture_specular1", 0);
		shader->SetInt("texture_normal1", 0);
	}
}

void Mesh::PreRender() {
	
	if (materialName != "") {
		Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
		material->PreRender();
	}
	else {

	}
}

void Mesh::Render() {
	if (type == MESH_TYPE::MODEL) {
		Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
		material->Render();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		material->GetShader()->Stop();
		glBindVertexArray(0);
	}
	else {
		if (materialName != "") {
			Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
			material->Render();
		}
		else {
			ShaderManager::GetInstance()->GetShader(std::string("defaultColour"))->Use();
		}
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
		
	}
}

void Mesh::RenderLowDetail() {
	if (type == MESH_TYPE::MODEL) {
		Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
		material->Render();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	else {
		if (materialName != "") {
			Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
			material->Render();
		}
		else {
			ShaderManager::GetInstance()->GetShader(std::string("plainColour"))->Use();
		}
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
	}
}

void Mesh::RenderDepth() {
	Shader* shader = ShaderManager::GetInstance()->GetShader(std::string("defaultShadowDepth"));
	shader->Use();

	if (type == MESH_TYPE::MODEL) {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	else {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
	}
	shader->Stop();
}

void Mesh::PostRender() {

	/*glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);*/

	if (materialName != "") {
		MaterialManager::GetInstance()->GetMaterial(materialName)->PostRender();
	}
}

std::vector<GLfloat> Mesh::GetVertices() {
	return vertices;
}

glm::vec3 Mesh::GetOffset() {
	return offset;
}