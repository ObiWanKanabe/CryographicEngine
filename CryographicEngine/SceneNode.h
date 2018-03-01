#ifndef SCENENODE_H
#define SCENENODE_H

#include <vector>
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include "Object.h"

class SceneNode {
private:
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;
	SceneNode *firstChild;
	SceneNode *lastChild;
	SceneNode *prevSibling;
	SceneNode *nextSibling;
	SceneNode *parent;
	std::vector<Object*> objectList;
public:
	typedef std::vector<Object*>::iterator objectIterator;

	SceneNode();
	~SceneNode();

	SceneNode(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale);

	SceneNode(SceneNode *other);

	void SetPosition(glm::vec3 _position);
	glm::vec3 GetPosition();
	glm::vec3 GetWorldPosition();

	void SetRotation(glm::vec3 _rotation);
	void SetRotation(glm::quat _rotation);
	glm::quat GetRotation();

	void SetScale(glm::vec3 _scale);
	glm::vec3 GetScale();

	void AttachChild(SceneNode *node);
	void RemoveChild(SceneNode *node);

	void AddObject(Object *object);
	void RemoveObject(Object *object);
	int GetNumberObjects();

	SceneNode* GetFirstChild();
	SceneNode* GetNextSibling();

	objectIterator ObjectBegin();
	objectIterator ObjectEnd();
};

#endif
