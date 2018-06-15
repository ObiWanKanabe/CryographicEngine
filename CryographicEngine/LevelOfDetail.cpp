#include "LevelOfDetail.h"

LevelOfDetailNode::LevelOfDetailNode(std::string _name, OBJECT_TYPE _type, float _distance) {
	name = _name;
	type = _type;
	distance = _distance;
	next = nullptr;
}

bool LevelOfDetailNode::IsCurrentLODNode(float distanceToCamera) {
	if (next == nullptr) {
		return true;
	}

	if (distance <= distanceToCamera && next->distance >= distanceToCamera) {
		return true;
	}
	else {
		return false;
	}
}

void LevelOfDetailNode::SetName(std::string _name) {
	name = _name;
}

void LevelOfDetailNode::SetType(OBJECT_TYPE _type) {
	type = _type;
}

void LevelOfDetailNode::SetDistance(float _distance) {
	distance = _distance;
}

void LevelOfDetailNode::SetNext(LevelOfDetailNode* _node) {
	next = _node;
}

std::string LevelOfDetailNode::GetName() {
	return name;
}

OBJECT_TYPE LevelOfDetailNode::GetType() {
	return type;
}

float LevelOfDetailNode::GetDistance() {
	return distance;
}

LevelOfDetailNode* LevelOfDetailNode::GetNext() {
	return next;
}

LevelOfDetail::LevelOfDetail(Mesh *_root) {
	root = new LevelOfDetailNode(_root->GetName(), MESH_LOD, 0.0f);
}

LevelOfDetail::LevelOfDetail(Model *_root) {
	root = new LevelOfDetailNode(_root->GetName(), MODEL_LOD, 0.0f);;
}

void LevelOfDetail::AddNextLOD(Mesh *mesh, float distanceToCamera) {
	LevelOfDetailNode* node = root;
	while (node->GetNext() != nullptr) {
		node = node->GetNext();
	}
	LevelOfDetailNode* newNode = new LevelOfDetailNode(mesh->GetName(), MESH_LOD, distanceToCamera);
	node->SetNext(newNode);
}

void LevelOfDetail::AddNextLOD(Model *model, float distanceToCamera) {
	LevelOfDetailNode* node = root;
	while (node->GetNext() != nullptr) {
		node = node->GetNext();
	}
	LevelOfDetailNode* newNode = new LevelOfDetailNode(model->GetName(), MODEL_LOD, distanceToCamera);
	node->SetNext(newNode);
}

OBJECT_TYPE LevelOfDetail::GetFirstLODType() {
	return root->GetType();
}

std::string LevelOfDetail::GetFirstLODName() {
	return root->GetName();
}

LevelOfDetailNode* LevelOfDetail::GetFirstLODNode() {
	return root;
}

OBJECT_TYPE LevelOfDetail::GetCurrentLODType(float distanceToCamera) {
	return GetCurrentLODNode(root, distanceToCamera)->GetType();
}

std::string LevelOfDetail::GetCurrentLODName(float distanceToCamera) {
	return GetCurrentLODNode(root, distanceToCamera)->GetName();
}

LevelOfDetailNode* LevelOfDetail::GetCurrentLODNode(LevelOfDetailNode* node, float distanceToCamera) {
	LevelOfDetailNode *it = node;
	while (!it->IsCurrentLODNode(distanceToCamera)) {
		it = it->GetNext();
	}
	return it;
}

OBJECT_TYPE LevelOfDetail::GetLastLODType() {
	return GetLastLODNode()->GetType();
}

std::string LevelOfDetail::GetLastLODName() {
	return GetLastLODNode()->GetName();
}

LevelOfDetailNode* LevelOfDetail::GetLastLODNode() {
	LevelOfDetailNode *it = root;
	while (it->GetNext() != nullptr) {
		it = it->GetNext();
	}
	return it;
}