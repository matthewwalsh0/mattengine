#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <initializer_list>
#include <vector>

namespace MattEngine {

class VertexAttribute {
public:
	int Type;
	int Count;

	VertexAttribute(int type, int count) : Type(type), Count(count) {}
};

class VertexArray {
public:
	VertexArray(const void* data, unsigned int vertexCount,
		const unsigned int* indices, unsigned int indexCount,
		std::initializer_list<VertexAttribute> attributes);
	void bind() const;

public:
	unsigned int IndexCount;

private:
	unsigned int m_arrayId;
	unsigned int m_bufferId;
	unsigned int m_indexBufferId;
	std::vector<VertexAttribute> m_attributes;
};

} // namespace MattEngine

#endif
