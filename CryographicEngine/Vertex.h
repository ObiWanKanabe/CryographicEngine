#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

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
	VertexComponentDescriptor() = delete;
	VertexComponentDescriptor(VertexComponentType _type, int _offset) : type(_type), offset(_offset) {};
	int getSize();
	int getNumFloats();
};

class VertexDescriptor {
public:
	std::vector<VertexComponentDescriptor> componentList;
	VertexDescriptor() { stride = 0; };
	~VertexDescriptor() {};
	void addComponent(VertexComponentDescriptor::VertexComponentType _type);
	int getSize() { return componentList.size(); }
	int getStride() { return stride; }
	std::vector<VertexComponentDescriptor> getComponentList() { return componentList; }
private:
	int stride;
};

#endif

