#include "Triangle.h"

Triangle::Triangle() {
	mesh = new Mesh(PRIMITIVE_TYPE::PLANE, ImageManager::GetInstance()->GetImage(std::string("derp")));
}

Triangle::~Triangle() {

}

void Triangle::Update() {

}

void Triangle::Render() {
	mesh->Render();
}