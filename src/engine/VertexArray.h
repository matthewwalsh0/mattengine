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
	VertexArray(const void* data, unsigned int count,
		std::initializer_list<VertexAttribute> attributes);
	void bind() const;

private:
	unsigned int m_arrayId;
	unsigned int m_bufferId;
	std::vector<VertexAttribute> m_attributes;
};

} // namespace MattEngine

#endif
