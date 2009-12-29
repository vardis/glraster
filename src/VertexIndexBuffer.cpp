/*
 * VertexIndexBuffer.cpp
 *
 *  Created on: Dec 26, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "VertexIndexBuffer.h"

VertexIndexBuffer::VertexIndexBuffer(GLenum primitiveType) :
	m_primitiveType(primitiveType), m_iboID(0), m_numIndices(0), m_usageHint(GL_STATIC_DRAW) {

	glGenBuffers(1, &m_iboID);
}

VertexIndexBuffer::~VertexIndexBuffer() {
	if (m_iboID && glIsBuffer(m_iboID)) {
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
	this->bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), 0, m_usageHint);
	m_numIndices = numIndices;
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

uint32_t* VertexIndexBuffer::mapData(GLenum accessType, uint32_t offset, uint32_t length) {
	assert(length <= m_numIndices);
	assert(accessType == GL_READ_ONLY || accessType == GL_WRITE_ONLY || accessType == GL_READ_WRITE);
	this->bind();
	if (offset > 0 || length > 0) {
		return (uint32_t*) glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, length, accessType);
	} else {
		return (uint32_t*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, accessType);
	}
}

void VertexIndexBuffer::unmapData() {
	this->bind();
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}
