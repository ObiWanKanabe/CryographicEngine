#include "SceneNode.h"

SceneNode::SceneNode() {
	firstChild = lastChild = prevSibling = nextSibling = parent = nullptr;
	position = glm::vec3(0.0f);
	rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f);
}

SceneNode::~SceneNode() {

}

SceneNode::SceneNode(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale) {
	firstChild = lastChild = prevSibling = nextSibling = parent = nullptr;
	position = _position;
	rotation = glm::quat(_rotation);
	scale = _scale;

}

SceneNode::SceneNode(SceneNode *other) {
	firstChild = lastChild = prevSibling = nextSibling = parent = nullptr;
	position = other->GetPosition();
	rotation = other->GetRotation();
	scale = other->GetScale();
}

void SceneNode::SetPosition(glm::vec3 _position) {
	position = _position;
}

glm::vec3 SceneNode::GetPosition() {
	return position;
}

glm::vec3 SceneNode::GetWorldPosition() {
	glm::vec3 result(0.0f);
	if (parent != nullptr) {
		result = parent->GetWorldPosition();
	}
	result += position;
	return result;
}

void SceneNode::SetRotation(glm::vec3 _rotation) {
	glm::quat rot;
	rot = glm::quat(glm::vec3(glm::radians(_rotation.x), glm::radians(_rotation.y), glm::radians(_rotation.z)));
	rotation = rot;
}

void SceneNode::SetRotation(glm::quat _rotation) {
	rotation = _rotation;
}

glm::quat SceneNode::GetRotation() {
	return rotation;
}

void SceneNode::SetScale(glm::vec3 _scale) {
	scale = _scale;
}

glm::vec3 SceneNode::GetScale() {
	return scale;
}

void SceneNode::AttachChild(SceneNode *node) {
	if (lastChild == nullptr) {
		firstChild = node;
	}
	else {
		lastChild->nextSibling = node;
		node->prevSibling = lastChild;
	}
	lastChild = node;
	node->parent = this;
}

void SceneNode::RemoveChild(SceneNode *node) {
	SceneNode *temp;

	for (temp = firstChild; temp != nullptr; temp = temp->nextSibling) {
		if (temp == node) {
			if (node->prevSibling) {
				node->prevSibling->nextSibling = node->nextSibling;
			}
			else {
				firstChild = node->nextSibling;
			}
			if (node->nextSibling) {
				node->nextSibling->prevSibling = node->prevSibling;
			}
			else {
				lastChild = node->prevSibling;
			}
			node->parent = nullptr;
			return;
		}
	}
}

void SceneNode::AddObject(Object *object) {
	objectList.push_back(object);
}

void SceneNode::RemoveObject(Object *object) {
	std::vector<Object*>::iterator it = std::find(objectList.begin(), objectList.end(), object);
	if (it != objectList.end()) {
		objectList.erase(it);
	}
}

int SceneNode::GetNumberObjects() {
	return objectList.size();
}

SceneNode* SceneNode::GetFirstChild() {
	return firstChild;
}

SceneNode* SceneNode::GetNextSibling() {
	return nextSibling;
}

SceneNode::objectIterator SceneNode::ObjectBegin() {
	return objectList.begin();
}

SceneNode::objectIterator SceneNode::ObjectEnd() {
	return objectList.end();
}