#include "Vertex.h"

int VertexComponentDescriptor::getSize()
{
	int result = 0;

	switch (type)
	{
	case VERTEX_UV:
		result = 2 * sizeof(float);
		break;
	case VERTEX_POSITION:
	case VERTEX_COLOUR3:
	case VERTEX_NORMAL:
		result = 3 * sizeof(float);
		break;
	case VERTEX_COLOUR4:
		result = 4 * sizeof(float);
		break;
	}

	return result;
}

int VertexComponentDescriptor::getNumFloats() {
	int result = 0;

	switch (type)
	{
	case VERTEX_UV:
		result = 2;
		break;
	case VERTEX_POSITION:
	case VERTEX_COLOUR3:
	case VERTEX_NORMAL:
		result = 3;
		break;
	case VERTEX_COLOUR4:
		result = 4;
		break;
	}

	return result;
}

VertexDescriptor::VertexDescriptor() {
	stride = 0;
}
void VertexDescriptor::addComponent(VertexComponentDescriptor::VertexComponentType _type) {
	VertexComponentDescriptor comp;

	comp.type = _type;
	comp.offset = stride;
	stride += comp.getSize();
	componentList.push_back(comp);
}
