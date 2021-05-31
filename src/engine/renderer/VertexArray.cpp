#include "VertexArray.h"

#include "Log.h"

#include <glad/glad.h>

using namespace MattEngine;

namespace Utils {

static unsigned int getAttributeTypeSize(int type) {
	switch (type) {
	case GL_FLOAT:
		return sizeof(float);
	case GL_INT:
		return sizeof(int);
	default:
		MATTENGINE_ASSERT(
			false, "Unrecognised attribute type in vertex array: %d", type);
	}
}

} // namespace Utils

VertexArray::VertexArray(const void* data, unsigned int count,
	const unsigned int* indices, unsigned int indexCount,
	std::initializer_list<VertexAttribute> attributes)
	: m_attributes(attributes), IndexCount(indexCount) {

	glGenVertexArrays(1, &m_arrayId);
	glBindVertexArray(m_arrayId);

	int totalSize = 0;

	for (auto& attribute : m_attributes) {
		totalSize +=
			Utils::getAttributeTypeSize(attribute.Type) * attribute.Count;
	}

	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(GL_ARRAY_BUFFER, count * totalSize, data, GL_STATIC_DRAW);

	if (indexCount > 0) {
		glGenBuffers(1, &m_indexBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int),
			indices, GL_STATIC_DRAW);
	}

	int attributeIndex = 0;
	unsigned int offset = 0;

	for (auto& attribute : m_attributes) {
		glEnableVertexAttribArray(attributeIndex);

		if (attribute.Type == GL_FLOAT) {
			glVertexAttribPointer(attributeIndex, attribute.Count,
				attribute.Type, GL_FALSE, totalSize,
				(const void*)(intptr_t)offset);
		} else if (attribute.Type == GL_INT) {
			glVertexAttribIPointer(attributeIndex, attribute.Count,
				attribute.Type, totalSize, (const void*)(intptr_t)offset);
		}

		attributeIndex++;
		offset += Utils::getAttributeTypeSize(attribute.Type) * attribute.Count;
	}
}

void VertexArray::bind() const { glBindVertexArray(m_arrayId); }