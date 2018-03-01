#include "Mesh.h"

Mesh::Mesh(std::vector<GLfloat> _vertices, std::vector<unsigned int> _indices, std::vector <Texture> _textures) {
	vertices = _vertices;
	indices = _indices;
	textures = _textures;
	vertexDescriptor = new VertexDescriptor();
	vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
	vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
	vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
	type = MESH_TYPE::MODEL;
	GenerateBuffers();
}

Mesh::Mesh(MESH_TYPE primType) {
	vertexDescriptor = new VertexDescriptor();
	type = primType;
	switch (primType) {
	case PLANE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
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
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertices = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.8f,
			0.5f, -0.5f, 0.0f,  0.8f, 0.8f, 0.8f,
			0.0f,  0.5f, 0.0f,  0.8f, 0.8f, 0.8f
		};
		break;
	case CUBE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
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
	vertexDescriptor = new VertexDescriptor();
	type = primType;
	switch (primType) {
	case PLANE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
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
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
		vertices = {
			-0.5f, -0.5f, 0.0f, r, g, b,
			0.5f, -0.5f, 0.0f,  r, g, b,
			0.0f,  0.5f, 0.0f,  r, g, b
		};
		break;
	case CUBE:
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
		vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
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
	materialName = material->GetName();
	vertexDescriptor = new VertexDescriptor();
	type = primType;
	if (material->GetType() == MATERIAL_TYPE::TEXTURE) {
		switch (primType) {
		case PLANE:
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
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
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
			vertices = {
				-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 
				0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  
				0.0f,  0.5f, 0.0f,  0.5f, 0.0f  
			};
			break;
		case CUBE:
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_UV);
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
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
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
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
			vertices = {
				-0.5f, -0.5f, 0.0f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.0f,  0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z
			};
			break;
		case CUBE:
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_COLOUR3);
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
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
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
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
			vertices = {
				-0.5f, -0.5f, 0.0f, material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.5f, -0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z,
				0.0f,  0.5f, 0.0f,  material->GetColour().x, material->GetColour().y, material->GetColour().z
			};
			break;
		case CUBE:
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_POSITION);
			vertexDescriptor->AddComponent(VertexComponentDescriptor::VertexComponentType::VERTEX_NORMAL);
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
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete vertexDescriptor;
	vertexDescriptor = nullptr;
}

void Mesh::SetName(std::string& _name) {
	name = _name;
}

std::string Mesh::GetName() {
	return name;
}

void Mesh::AddComponent(VertexComponentDescriptor::VertexComponentType _type) {
	vertexDescriptor->AddComponent(_type);
}

void Mesh::GenerateBuffers() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	if (type == MESH_TYPE::MODEL) {
		glGenBuffers(1, &EBO);
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	if (type == MESH_TYPE::MODEL) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}

	for (int i = 0; i < vertexDescriptor->GetSize(); i++) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, vertexDescriptor->componentList[i].GetNumFloats(), GL_FLOAT, GL_FALSE, vertexDescriptor->GetStride(), (void*)(vertexDescriptor->componentList[i].offset));
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		Shader* shader = ShaderManager::GetInstance()->GetShader(std::string("INSERTMODELSHADERHERE"));
		shader->use();
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
											 
			std::string number;
			std::string name = textures[i].type;

			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			shader->SetFloat(("material." + name + number).c_str(), i);
			SetMat4("model", modelMatrix);
			SetMat4("view", viewMatrix);
			SetMat4("projection", projectionMatrix);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
		}
	}
	else {
		if (MaterialManager::GetInstance()->GetMaterial(materialName) != nullptr) {
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
}

void Mesh::PreRender() {
	if (MaterialManager::GetInstance()->GetMaterial(materialName) != nullptr) {
		Material* material = MaterialManager::GetInstance()->GetMaterial(materialName);
		material->PreRender();
	}
	else {

	}
}

void Mesh::Render() {
	if (type == MESH_TYPE::MODEL) {
		Shader* shader = ShaderManager::GetInstance()->GetShader(std::string("INSERTMODELSHADERHERE"));
		shader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		if (MaterialManager::GetInstance()->GetMaterial(materialName) != nullptr) {
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
	if (MaterialManager::GetInstance()->GetMaterial(materialName) != nullptr) {
		MaterialManager::GetInstance()->GetMaterial(materialName)->PostRender();
	}
}

std::vector<GLfloat> Mesh::GetVertices() {
	return vertices;
}