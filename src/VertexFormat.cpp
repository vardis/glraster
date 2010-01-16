/*
 * VertexFormat.cpp
 *
 *  Created on: Nov 27, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "VertexFormat.h"

VertexFormat::VertexFormat() :
	m_numElements(0), m_elements() {

}

VertexFormat::~VertexFormat() {
}

VertexFormat& VertexFormat::operator=(const VertexFormat& rhs) {
	m_numElements = rhs.m_numElements;
	for (uint8_t i = 0; i < m_numElements; i++) {
		// copy pointers
		m_elements[i] = rhs.m_elements[i];
	}
	return *this;
}

VertexElement* VertexFormat::addElement(VertexElement* ve) {
	m_elements[m_numElements] = VertexElementPtr(ve);
	++m_numElements;
	return ve;
}

VertexElement* VertexFormat::addElement(VertexElementSemantic semantic, VertexElementFormat format, uint32_t offset,
		void* data) {
	assert(m_numElements <= MAX_VERTEX_ELEMENTS);
	return this->addElement(new VertexElement(semantic, format, offset, data));
}

VertexElement* VertexFormat::addElement(VertexElementSemantic semantic, VertexElementFormat format) {
	assert(m_numElements <= MAX_VERTEX_ELEMENTS);
	return this->addElement(new VertexElement(semantic, format, 0, 0));
}

VertexElement* VertexFormat::getElementByIndex(uint8_t numElement) {
	assert(numElement < MAX_VERTEX_ELEMENTS);
	return m_elements[numElement].get();
}

VertexElement* VertexFormat::getElementBySemantic(VertexElementSemantic semantic) {
	for (uint8_t i = 0; i < m_numElements; i++) {
		if (m_elements[i]->m_semantic == semantic) {
			return m_elements[i].get();
		}
	}
	return 0;
}

uint8_t VertexFormat::getNumElements() const {
	return m_numElements;
}

void VertexFormat::printData() {
	for (uint8_t i = 0; i < m_numElements; i++) {
		m_elements[i];
	}
}
