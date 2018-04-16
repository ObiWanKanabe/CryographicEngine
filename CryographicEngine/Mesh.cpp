#include "Mesh.h"

Mesh::Mesh(std::vector<GLfloat> _vertices, std::vector<unsigned int> _indices, std::vector <Texture> _textures) {
	type = MESH_TYPE::MODEL;
	vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
	vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
	vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
	vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_TANGENT);
	vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_BITANGENT);
	this->vertices = _vertices;
	this->indices = _indices;
	this->textures = _textures;
	GenerateBuffers();
}

Mesh::Mesh(MESH_TYPE primType) {
	name = "";
	type = primType;
	switch (primType) {
	case PLANE:
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
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
		vertices = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.8f,
			0.5f, -0.5f, 0.0f,  0.8f, 0.8f, 0.8f,
			0.0f,  0.5f, 0.0f,  0.8f, 0.8f, 0.8f
		};
		break;
	case CUBE:
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertices = {
			-0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,
			0.5f,  0.5f, -0.5f,   0.8f, 0.8f, 0.8f,
			0.5f, -0.5f, -0.5f,   0.8f, 0.8f, 0.8f,
			0.5f,  0.5f, -0.5f,   0.8f, 0.8f, 0.8f,
			-0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,
			-0.5f,  0.5f, -0.5f,  0.8f, 0.8f, 0.8f,
			
			-0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.8f,
			0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.8f,
			0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,
			0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,
			-0.5f,  0.5f,  0.5f, 0.8f, 0.8f, 0.8f,
			-0.5f, -0.5f,  0.5f, 0.8f, 0.8f, 0.8f,

			-0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,
			-0.5f,  0.5f, -0.5f,  0.8f, 0.8f, 0.8f,
			-0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,
			-0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,
			-0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.8f,
			-0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,

			0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,
			0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,
			0.5f,  0.5f, -0.5f,  0.8f, 0.8f, 0.8f,
			0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,
			0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,
			0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.8f,

			-0.5f, -0.5f, -0.5f, 0.8f, 0.8f, 0.8f,
			0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.8f,
			0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.8f,
			0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.8f,
			-0.5f, -0.5f,  0.5f, 0.8f, 0.8f, 0.8f,
			-0.5f, -0.5f, -0.5f, 0.8f, 0.8f, 0.8f,

			-0.5f,  0.5f, -0.5f, 0.8f, 0.8f, 0.8f,
			0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,
			0.5f,  0.5f, -0.5f,  0.8f, 0.8f, 0.8f,
			0.5f,  0.5f,  0.5f,  0.8f, 0.8f, 0.8f,
			-0.5f,  0.5f, -0.5f, 0.8f, 0.8f, 0.8f,
			-0.5f,  0.5f,  0.5f, 0.8f, 0.8f, 0.8f
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
	switch (primType) {
	case PLANE:
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
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
		vertices = {
			-0.5f, -0.5f, 0.0f, r, g, b,
			0.5f, -0.5f, 0.0f,  r, g, b,
			0.0f,  0.5f, 0.0f,  r, g, b
		};
		break;
	case CUBE:
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertices = {
			-0.5f, -0.5f, -0.5f,  r, g, b,
			0.5f,  0.5f, -0.5f,   r, g, b,
			0.5f, -0.5f, -0.5f,   r, g, b,
			0.5f,  0.5f, -0.5f,   r, g, b,
			-0.5f, -0.5f, -0.5f,  r, g, b,
			-0.5f,  0.5f, -0.5f,  r, g, b,

			-0.5f, -0.5f,  0.5f,  r, g, b,
			0.5f, -0.5f,  0.5f,  r, g, b,
			0.5f,  0.5f,  0.5f,  r, g, b,
			0.5f,  0.5f,  0.5f,  r, g, b,
			-0.5f,  0.5f,  0.5f, r, g, b,
			-0.5f, -0.5f,  0.5f, r, g, b,

			-0.5f,  0.5f,  0.5f,  r, g, b,
			-0.5f,  0.5f, -0.5f,  r, g, b,
			-0.5f, -0.5f, -0.5f,  r, g, b,
			-0.5f, -0.5f, -0.5f,  r, g, b,
			-0.5f, -0.5f,  0.5f,  r, g, b,
			-0.5f,  0.5f,  0.5f,  r, g, b,

			0.5f,  0.5f,  0.5f,  r, g, b,
			0.5f, -0.5f, -0.5f,  r, g, b,
			0.5f,  0.5f, -0.5f,  r, g, b,
			0.5f, -0.5f, -0.5f,  r, g, b,
			0.5f,  0.5f,  0.5f,  r, g, b,
			0.5f, -0.5f,  0.5f,  r, g, b,

			-0.5f, -0.5f, -0.5f, r, g, b,
			0.5f, -0.5f, -0.5f,  r, g, b,
			0.5f, -0.5f,  0.5f,  r, g, b,
			0.5f, -0.5f,  0.5f,  r, g, b,
			-0.5f, -0.5f,  0.5f, r, g, b,
			-0.5f, -0.5f, -0.5f, r, g, b,

			-0.5f,  0.5f, -0.5f, r, g, b,
			0.5f,  0.5f,  0.5f,  r, g, b,
			0.5f,  0.5f, -0.5f,  r, g, b,
			0.5f,  0.5f,  0.5f,  r, g, b,
			-0.5f,  0.5f, -0.5f, r, g, b,
			-0.5f,  0.5f,  0.5f, r, g, b
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
	if (material->GetType() == MATERIAL_TYPE::TEXTURE) {
		switch (primType) {
		case PLANE:
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
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
			vertices = {
				-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 
				0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  
				0.0f,  0.5f, 0.0f,  0.5f, 0.0f  
			};
			break;
		case CUBE:
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
			vertices = {
				-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 
				0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 
				0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 
				0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 
				-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 
				-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 
												  
				-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 
				 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
				 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
				 -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
													
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
				-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 
				-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
													
				 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 
				 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 
				 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  
				 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 
				 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 
				 0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  
													
				 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 
				 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
													
				 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 
				 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 
				 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
				 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f  
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
			vertices = {
				-0.5f, -0.5f, 0.0f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.0f,  0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z
			};
			break;
		case CUBE:
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor.AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
			vertices = {
				-0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f,  0.5f, -0.5f,   material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f, -0.5f,   material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f,  0.5f, -0.5f,   material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f,  0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,

				-0.5f, -0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f,  0.5f,  0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f, -0.5f,  0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,

				-0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f,  0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f, -0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,

				0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f,  0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,

				-0.5f, -0.5f, -0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f, -0.5f,  0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f, -0.5f, -0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,

				-0.5f,  0.5f, -0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f,  0.5f, -0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f,  0.5f,  0.5f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f,  0.5f, -0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				-0.5f,  0.5f,  0.5f, material->GetColour().x, material->GetColour().y, material->GetColour().z
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

std::string Mesh::GetName() {
	return name;
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

		glBindVertexArray(0);

	}
	else {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

		for (int i = 0; i < vertexDescriptor.GetSize(); i++) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, vertexDescriptor.componentList[i].GetNumFloats(), GL_FLOAT, GL_FALSE, vertexDescriptor.GetStride(), (void*)(vertexDescriptor.componentList[i].offset));
		}

		glBindVertexArray(0);
	}
}

void Mesh::SetMat4(const std::string &name, const glm::mat4 &mat) {
	if (MaterialManager::GetInstance()->GetMaterial(materialName) != nullptr) {
		MaterialManager::GetInstance()->GetMaterial(materialName)->GetShader()->SetMat4(name, mat);
	}
	else {
		ShaderManager::GetInstance()->GetShader(std::string("defaultColour"))->SetMat4(name, mat);
	}
}

void Mesh::SetVec3(const std::string &name, const glm::vec3 &vec) {
	if (MaterialManager::GetInstance()->GetMaterial(materialName) != nullptr) {
		MaterialManager::GetInstance()->GetMaterial(materialName)->GetShader()->SetVec3(name, vec);
	}
	else {
		ShaderManager::GetInstance()->GetShader(std::string("defaultColour"))->SetVec3(name, vec);
	}
}

void Mesh::BindUniforms(Camera *camera, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	if (type == MESH_TYPE::MODEL) {
		Shader *shader = ShaderManager::GetInstance()->GetShader(std::string("defaultModel"));
		
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		if (textures.size() != 0) {
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
												 
				std::string number;
				std::string name = textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);
				else if (name == "texture_normal")
					number = std::to_string(normalNr++);
				else if (name == "texture_height")
					number = std::to_string(heightNr++);

														
				shader->SetInt((name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].ID);
			}
			if (textures.size() == 1) {
				shader->SetInt("texture_specular1", 0);
				shader->SetInt("texture_normal1", 0);
			}
		}
		else {
			shader->SetInt("texture_diffuse1", 0);
			shader->SetInt("texture_specular1", 0);
			shader->SetInt("texture_normal1", 0);
		}
	} else if (MaterialManager::GetInstance()->GetMaterial(materialName) != nullptr) {
		Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
		material->BindUniforms();
		SetMat4("model", modelMatrix);
		SetMat4("view", viewMatrix);
		SetMat4("projection", projectionMatrix);
		SetVec3("cameraPos", camera->GetPosition());
	}
	else {
		Shader* shader = ShaderManager::GetInstance()->GetShader(std::string("defaultColour"));
		shader->use();
		shader->SetMat4("model", modelMatrix);
		shader->SetMat4("view", viewMatrix);
		shader->SetMat4("projection", projectionMatrix);
	}
}

void Mesh::BindUniforms(Shader* shader) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	if (textures.size() != 0) {
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
											 
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);

													
			shader->SetInt((name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
		}
		if (textures.size() == 1) {
			shader->SetInt("texture_specular1", 0);
			shader->SetInt("texture_normal1", 0);
		}
	}
	else {
		shader->SetInt("texture_diffuse1", 0);
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
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		if (materialName != "") {
			Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
			material->Render();
		}
		else {
			ShaderManager::GetInstance()->GetShader(std::string("defaultColour"))->use();
		}
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
}

void Mesh::PostRender() {
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	if (materialName != "") {
		MaterialManager::GetInstance()->GetMaterial(materialName)->PostRender();
	}
}

std::vector<GLfloat> Mesh::GetVertices() {
	return vertices;
}