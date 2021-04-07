#include "VertexArray.h"

#include "Log.h"

#include <glad/glad.h>

using namespace MattEngine;

namespace Utils {

static unsigned int getAttributeTypeSize(int type) {
	switch (type) {
	case GL_FLOAT:
		return sizeof(float);
	default:
		MATTENGINE_ASSERT(false, "Unrecognised attribute type: %d", type);
	}
}

} // namespace Utils

VertexArray::VertexArray(const void* data, unsigned int count,
	std::initializer_list<VertexAttribute> attributes)
	: m_attributes(attributes) {

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

	int attributeIndex = 0;
	unsigned int offset = 0;

	for (auto& attribute : m_attributes) {
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, attribute.Count, attribute.Type,
			GL_FALSE, totalSize, (const void*)(intptr_t)offset);

		attributeIndex++;
		offset += Utils::getAttributeTypeSize(attribute.Type) * attribute.Count;
	}
}

void VertexArray::bind() const { glBindVertexArray(m_arrayId); }