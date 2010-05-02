/*
 * VertexAttributeBuffer.cpp
 *
 *  Created on: Dec 26, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "VertexFormat.h"
#include "VertexAttributeBuffer.h"

VertexAttributeBuffer::VertexAttributeBuffer(VertexAttribute* ve) :
	m_vboID(0), m_ve(ve), m_numElements(0), m_usageHint(GL_STATIC_DRAW), m_mapped(0) {
	glGenBuffers(1, &m_vboID);
}

VertexAttributeBuffer::~VertexAttributeBuffer() {
	if (m_vboID && glIsBuffer(m_vboID)) {
		unmapData();
		glDeleteBuffers(1, &m_vboID);
	}
}

void VertexAttributeBuffer::setData(void* data, uint32_t numElements) {
	allocate(numElements);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numElements * VertexAttribute::getFormatSize(m_ve->m_format), data);
	m_numElements = numElements;
}

void VertexAttributeBuffer::updateSubData(uint32_t offset, uint32_t count, void* data) {
	void* mapped = mapSubData(GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT, offset, count);
	if (mapped) {
		memcpy(mapped, data, count * VertexAttribute::getFormatSize(m_ve->m_format));
		unmapData();
	} else {
		// TODO: error reporting
		std::cerr << "Failed to map VBO data\n";
	}
}

void VertexAttributeBuffer::allocate(uint32_t numElements) {
	if (numElements != m_numElements) {
		this->bind();
		glBufferData(GL_ARRAY_BUFFER, numElements * VertexAttribute::getFormatSize(m_ve->m_format), 0, m_usageHint);
		m_numElements = numElements;
	}
}

void VertexAttributeBuffer::deallocate() {
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, 0, 0, m_usageHint);
	m_numElements = 0;
}

void VertexAttributeBuffer::bind() {
	if (m_vboID && glIsBuffer(m_vboID)) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	}
}

void VertexAttributeBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexAttributeBuffer::bindAttributeData(GLuint attributeIndex) {

	this->bind();
	glVertexAttribPointer(attributeIndex, VertexAttribute::getFormatElementCount(m_ve->m_format),
			VertexAttribute::getFormatType(m_ve->m_format), false, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(attributeIndex);
}


void* VertexAttributeBuffer::mapData(GLenum accessType) {
	assert(!m_mapped);

	if (m_mapped) {
		return 0;
	}

	this->bind();
	m_mapped = glMapBuffer(GL_ARRAY_BUFFER, accessType);
	return m_mapped;
}

void* VertexAttributeBuffer::mapSubData(GLbitfield accessType, uint32_t offset, uint32_t count) {
	assert(count <= m_numElements && !m_mapped);

	if (m_mapped) {
		return 0;
	}

	GLenum bitAccess = accessType | GL_MAP_INVALIDATE_RANGE_BIT;
	this->bind();
	if (count == 0) {
		count = m_numElements;
		offset = 0;
	}

	uint32_t buffOffset = offset * VertexAttribute::getFormatSize(m_ve->m_format);
	uint32_t buffSize = count * VertexAttribute::getFormatSize(m_ve->m_format);
	m_mapped = glMapBufferRange(GL_ARRAY_BUFFER, buffOffset, buffSize, bitAccess);
	return m_mapped;
}

void VertexAttributeBuffer::unmapData() {
	if (m_mapped) {
		this->bind();
		glUnmapBuffer(GL_ARRAY_BUFFER);
		m_mapped = 0;
	}
}

void VertexAttributeBuffer::printData() {
	using std::cout;
	bool unmap = false;
	if (!m_mapped) {
		this->mapData(GL_READ_ONLY);
		unmap = true;
	}

	float* fp;
	short* up;
	uint8_t* bp;
	for (uint32_t i = 0; i < m_numElements; i++) {
		switch (m_ve->m_format) {
		case VertexFormat_FLOAT1:
		case VertexFormat_FLOAT2:
		case VertexFormat_FLOAT3:
		case VertexFormat_FLOAT4:
			fp = (float*) m_mapped;
			fp += i;
			cout << "f(";
			for (int j = 0; j < m_ve->getFormatElementCount(m_ve->m_format); j++) {
				cout << *fp++ << " ";
			}
			cout << ")\n";
			break;
		case VertexFormat_SHORT1:
		case VertexFormat_SHORT2:
		case VertexFormat_SHORT3:
		case VertexFormat_SHORT4:
			up = (short*) m_mapped;
			up += i;
			break;
		case VertexFormat_UBYTE3:
		case VertexFormat_UBYTE4:
			bp = (uint8_t*) m_mapped;
			bp += i;

			break;
		default:
			//TODO: error reporting
			break;
		}
	}

	if (unmap) {
		this->unmapData();
	}
}
