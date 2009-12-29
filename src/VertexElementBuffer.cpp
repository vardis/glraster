/*
 * VertexElementBuffer.cpp
 *
 *  Created on: Dec 26, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "VertexFormat.h"
#include "VertexElementBuffer.h"

VertexElementBuffer::VertexElementBuffer(VertexElement* ve) :
	m_vboID(0), m_ve(ve), m_numElements(0), m_usageHint(GL_STATIC_DRAW) {
	glGenBuffers(1, &m_vboID);
}

VertexElementBuffer::~VertexElementBuffer() {
	if (m_vboID && glIsBuffer(m_vboID)) {
		glDeleteBuffers(1, &m_vboID);
	}
}

void VertexElementBuffer::setData(void* data, uint32_t numElements) {
	allocate(numElements);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numElements * VertexElement::getFormatSize(m_ve->m_format), data);
	m_numElements = numElements;
}

void VertexElementBuffer::allocate(uint32_t numElements) {
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, numElements * VertexElement::getFormatSize(m_ve->m_format), 0, m_usageHint);
}

void VertexElementBuffer::deallocate() {
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, 0, 0, m_usageHint);
	m_numElements = 0;
}

void VertexElementBuffer::bind() {
	if (m_vboID && glIsBuffer(m_vboID)) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	}
}

void VertexElementBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexElementBuffer::uploadData() {

	this->bind();

	switch (m_ve->m_semantic) {
	case Vertex_Pos:
		glEnableClientState( GL_VERTEX_ARRAY);
		glVertexPointer(VertexElement::getFormatElementCount(m_ve->m_format), VertexElement::getFormatType(
				m_ve->m_format), 0, BUFFER_OFFSET(0));
		break;

	case Vertex_Color:
		glEnableClientState( GL_COLOR_ARRAY);
		glColorPointer(VertexElement::getFormatElementCount(m_ve->m_format), VertexElement::getFormatType(
				m_ve->m_format), 0, BUFFER_OFFSET(0));
		break;
	case Vertex_Normal:
		glEnableClientState( GL_NORMAL_ARRAY);
		glNormalPointer(VertexElement::getFormatType(m_ve->m_format), 0, BUFFER_OFFSET(0));
		break;
	case Vertex_TexCoord0:
	case Vertex_TexCoord1:
	case Vertex_TexCoord2:
	case Vertex_TexCoord3:
	case Vertex_TexCoord4:
	case Vertex_TexCoord5:
	case Vertex_TexCoord6:
	case Vertex_TexCoord7:
		glEnableClientState( GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(VertexElement::getFormatElementCount(m_ve->m_format), VertexElement::getFormatType(
				m_ve->m_format), 0, BUFFER_OFFSET(0));
	default:
		//TODO: error reporting
		break;
	}
}
