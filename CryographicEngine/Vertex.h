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
		VERTEX_UV,
		VERTEX_TANGENT,
		VERTEX_BITANGENT
	};

	// Component must have a specfic type and offset in the Vertex Descriptor list
	VertexComponentType type;
	int offset;

	// Deleted default constructor
	VertexComponentDescriptor() = delete;

	// Constructor requires a type, and offset
	VertexComponentDescriptor(VertexComponentType _type, int _offset) : type(_type), offset(_offset) {};

	// Returns size of component in bytes
	int GetSize();

	// Returns number of floats in the component
	int GetNumFloats();
};

class VertexDescriptor {
public:

	// List of all Vertex Component Descriptors
	std::vector<VertexComponentDescriptor> componentList;

	// Constructor initializes stride to 0
	VertexDescriptor() : stride(0) {} ;

	// Deconstructor
	~VertexDescriptor() {};

	// Adding component requires a type, creates a new Vertex Component Descriptor, sets stride and pushes it into the component list
	void AddComponent(VertexComponentDescriptor::VertexComponentType _type);

	// Returns the size of the list
	int GetSize() { return componentList.size(); }

	// Returns the stride of the list
	int GetStride() { return stride; }

	// Returns the component list for use
	std::vector<VertexComponentDescriptor> GetComponentList() { return componentList; }
private:

	// Stride in bytes of all the components
	int stride;
};

#endif

