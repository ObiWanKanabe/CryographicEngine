#ifndef LEVELOFDETAIL_H
#define LEVELOFDETAIL_H

#include <string>
#include "ModelManager.h"
#include "MeshManager.h"

// This is an LOD system that can be given to GameObjects in place of a normal mesh/model
// This will provide better performance if every GameObject has a list of LODs with 
// numerous lower poly models to use at given distances

enum OBJECT_TYPE {
	MODEL_LOD,
	MESH_LOD,
	EMPTY_LOD
};

class LevelOfDetailNode {
public:
	LevelOfDetailNode(std::string _name, OBJECT_TYPE _type, float _distance);

	bool IsCurrentLODNode(float distanceToCamera);

	void SetName(std::string _name);
	void SetType(OBJECT_TYPE _type);
	void SetDistance(float _distance);
	void SetNext(LevelOfDetailNode* _node);

	std::string GetName();
	OBJECT_TYPE GetType();
	float GetDistance();
	LevelOfDetailNode* GetNext();

private:
	std::string name;
	OBJECT_TYPE type;
	float distance;
	LevelOfDetailNode* next;
};

class LevelOfDetail {
public:
	LevelOfDetail() = delete;

	LevelOfDetail(Mesh *_root);
	LevelOfDetail(Model *_root);

	void AddNextLOD(Mesh *mesh, float distanceToCamera);
	void AddNextLOD(Model *model, float distanceToCamera);

	OBJECT_TYPE GetFirstLODType();
	std::string GetFirstLODName();
	LevelOfDetailNode* GetFirstLODNode();

	OBJECT_TYPE GetCurrentLODType(float distanceToCamera);
	std::string GetCurrentLODName(float distanceToCamera);
	LevelOfDetailNode* GetCurrentLODNode(LevelOfDetailNode* node, float distanceToCamera);

	OBJECT_TYPE GetLastLODType();
	std::string GetLastLODName();
	LevelOfDetailNode* GetLastLODNode();

private:
	LevelOfDetailNode* root;
};

#endif