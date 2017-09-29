#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

struct VertexData {

};

class VertexComponentDescriptor {
public:
	enum VertexComponentType {
		VERTEX_NULL,
		VERTEX_POSITION,
		VERTEX_NORMAL,
		VERTEX_COLOUR3,
		VERTEX_COLOUR4,
		VERTEX_UV
	};
	VertexComponentType type;
	int offset;
	VertexComponentDescriptor() : type(VERTEX_NULL), offset(0) {};
	int getSize();
	int getNumFloats();
};

class VertexDescriptor {
public:
	std::vector<VertexComponentDescriptor> componentList;
	VertexDescriptor();
	~VertexDescriptor() {};
	void addComponent(VertexComponentDescriptor::VertexComponentType _type);
	int getSize() { return componentList.size(); }
	int getStride() { return stride; }
	std::vector<VertexComponentDescriptor> getComponentList() { return componentList; }
private:
	int stride;
};

#endif

