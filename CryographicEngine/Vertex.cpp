#include "Vertex.h"

int VertexComponentDescriptor::GetSize()
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

int VertexComponentDescriptor::GetNumFloats() {
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

void VertexDescriptor::AddComponent(VertexComponentDescriptor::VertexComponentType _type) {
	VertexComponentDescriptor comp(_type, stride);
	stride += comp.GetSize();
	componentList.push_back(comp);
}
