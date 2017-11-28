#include "Triangle.h"

Triangle::Triangle() {

	vertices = {
		-0.5f, -0.5f, 0.0f,
		1.0f, 0.4f, 0.0f,
		0.5f, -0.5f, 0.0f,
		1.0f, 0.6f, 0.3f,
		0.0f,  0.5f, 0.0f,
		1.0f, 0.8f, 0.6f,

	};

	/*vertices = {
		-0.5f, -0.5f, -0.5f, 1.0f, 0.4f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.4f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.4f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.4f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.4f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.4f, 0.0f,

		-0.5f, -0.5f,  0.5f, 1.0f, 0.4f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.4f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.4f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.4f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.4f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.4f, 0.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 0.6f, 0.3f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.6f, 0.3f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.6f, 0.3f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.6f, 0.3f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.6f, 0.3f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.6f, 0.3f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.3f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.3f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.6f, 0.3f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.6f, 0.3f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.3f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.3f,

		-0.5f, -0.5f, -0.5f, 1.0f, 0.8f, 0.6f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.8f, 0.6f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.8f, 0.6f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.8f, 0.6f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.8f, 0.6f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.8f, 0.6f,
									 
		-0.5f,  0.5f, -0.5f, 1.0f, 0.8f, 0.6f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.8f, 0.6f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.8f, 0.6f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.8f, 0.6f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.8f, 0.6f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.8f, 0.6f,
	};*/

	mesh.AddComponent(VertexComponentDescriptor::VERTEX_POSITION);
	mesh.AddComponent(VertexComponentDescriptor::VERTEX_COLOUR3);
	mesh.GenerateBuffers(vertices);
}

Triangle::~Triangle() {

}

void Triangle::Update() {

}

void Triangle::Render() {
	mesh.Render(vertices);
}