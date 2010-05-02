/*
 * VertexIndexBuffer.cpp
 *
 *  Created on: Dec 26, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "VertexIndexBuffer.h"

VertexIndexBuffer::VertexIndexBuffer(GLenum primitiveType) :
	m_primitiveType(primitiveType), m_iboID(0), m_numIndices(0), m_usageHint(GL_STATIC_DRAW), m_isMapped(false) {

	glGenBuffers(1, &m_iboID);
}

VertexIndexBuffer::~VertexIndexBuffer() {
	if (m_iboID && glIsBuffer(m_iboID)) {
		unmapData();
		glDeleteBuffers(1, &m_iboID);
	}
}

void VertexIndexBuffer::setData(uint32_t* data, uint32_t numIndices) {
	if (m_numIndices < numIndices) {
		this->allocate(numIndices);
	} else {
		this->bind();
	}
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_numIndices * sizeof(uint32_t), data);
}

void VertexIndexBuffer::setData(IndexArrayPtr data, uint32_t numIndices) {
	this->setData(data.get(), numIndices);
}

void VertexIndexBuffer::allocate(uint32_t numIndices) {
	if (numIndices != m_numIndices) {
		this->bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), 0, m_usageHint);
		m_numIndices = numIndices;
	}
}

void VertexIndexBuffer::deallocate() {
	this->bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, m_usageHint);
	m_numIndices = 0;
}

void VertexIndexBuffer::bind() {
	if (m_iboID && glIsBuffer(m_iboID)) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	}
}

void VertexIndexBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexIndexBuffer::drawPrimitives(uint16_t numInstances) {
	this->bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	if (numInstances > 1) {
		glDrawElementsInstancedARB(m_primitiveType, m_numIndices, GL_UNSIGNED_INT, 0, numInstances);
	} else {
		glDrawRangeElements(m_primitiveType, 0, m_numIndices - 1, m_numIndices, GL_UNSIGNED_INT, 0);
	}
}

uint32_t* VertexIndexBuffer::mapData(GLenum accessType) {
	assert(!m_isMapped);

	if (m_isMapped) {
		return 0;
	}

	this->bind();
	uint32_t* data = (uint32_t*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, accessType);
	if (data) {
		m_isMapped = true;
	} else {
		//TODO: error reporting
	}
	return data;
}

uint32_t* VertexIndexBuffer::mapSubData(GLbitfield accessType, uint32_t offset, uint32_t count) {
	assert(count <= m_numIndices && !m_isMapped);

	if (m_isMapped) {
		return 0;
	}

	GLenum bitAccess = accessType | GL_MAP_INVALIDATE_RANGE_BIT;
	this->bind();

	if (count == 0) {
		count = m_numIndices;
		offset = 0;
	}

	uint32_t buffOffset = offset * sizeof(uint32_t);
	uint32_t buffSize = count * sizeof(uint32_t);
	uint32_t* data = (uint32_t*) glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, buffOffset, buffSize, bitAccess);
	if (data) {
		m_isMapped = true;
	} else {
		//TODO: error reporting
	}
	return data;
}

void VertexIndexBuffer::unmapData() {
	if (m_isMapped) {
		this->bind();
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		m_isMapped = false;
	}
}

void VertexIndexBuffer::printData() {

	uint32_t* fp = this->mapData();
	std::cout << "Printing " << m_numIndices << " indices\n";
	for (uint32_t e = 0, c = 1; e < m_numIndices; e++) {
		if (!c)
			std::cout << "(";

		std::cout << *fp++ << ", ";

		if (c % 3 == 0) {
			std::cout << ")\n";
			c = 1;
		} else {
			++c;
		}
	}
	std::cout << ")\n";
	this->unmapData();

}
