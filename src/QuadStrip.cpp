/*
 * QuadStrip.cpp
 *
 *  Created on: Jan 3, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "QuadStrip.h"

QuadStrip::QuadStrip() :
	m_numQuads(0), m_numVertices(0), m_vf(), m_ibo(), m_currentQuad(0), m_isComplete(false) {

}

QuadStrip::~QuadStrip() {

}

void QuadStrip::specifyVertexFormat(const VertexFormat& vf) {
	m_vf.reset(new VertexFormat());
	(*m_vf) = vf;

	// reset counters for each vertex element
	for (uint8_t i = 0; i < m_vf->getNumElements(); i++) {
		VertexElement* ve = m_vf->getElementByIndex(i);
		m_sizePerSemantic[ve->m_semantic] = 0;
	}
	m_isComplete = false;
}

//void QuadStrip::specifyVertexElement(VertexElementSemantic semantic, VertexElementFormat format) {
//	VertexElement* ve = m_vf->getElementBySemantic(semantic);
//	if (ve && ve->m_format != format) {
//		ve->m_format = format;
//	} else if (!ve) {
//		m_vf->addElement(semantic, format, 0, 0);
//	} else {
//		return;
//	}
//	m_isSpecified = false;
//}

void QuadStrip::beginQuadStrip(uint32_t numQuads) {
	m_numQuads = numQuads;
	m_numVertices = 2 * numQuads + 2;
	for (uint8_t i = 0; i < m_vf->getNumElements(); i++) {
		VertexElement* ve = m_vf->getElementByIndex(i);
		ve->m_vbo->allocate(m_numVertices);
	}
}

bool QuadStrip::endQuadStrip() {
	// verify that the position semantic has been specified
	std::map<VertexElementSemantic, uint32_t>::const_iterator it = m_sizePerSemantic.find(Vertex_Pos);
	assert(it != m_sizePerSemantic.end());
	if (it == m_sizePerSemantic.end()) {
		return false;
	}

	// verify that all data have been specified
	it = m_sizePerSemantic.begin();
	while (it != m_sizePerSemantic.end()) {
		if ((*it).second != m_numVertices) {
			return false;
		}
		++it;
	}

	// upload vertex attributes
	for (uint8_t i = 0; i < m_vf->getNumElements(); i++) {
		VertexElement* ve = m_vf->getElementByIndex(i);
		ve->m_vbo->uploadData();
	}

	// build and upload indices
	m_ibo = VertexIndexBufferPtr(new VertexIndexBuffer(GL_QUAD_STRIP));
	m_ibo->allocate(m_numVertices);
	uint32_t* indices = m_ibo->mapSubData(GL_MAP_WRITE_BIT, 0, m_numVertices);
	for (uint32_t idx = 0; idx < m_numVertices; idx++) {
		indices[idx] = idx;
	}
	m_ibo->unmapData();

	// update bounding box
	m_bounds = AABB<float>::EmptyAABB();
	VertexElement* ve = m_vf->getElementBySemantic(Vertex_Pos);
	//TODO: !IMPORTANT! complete this...
	// data = vbo->mapData()
	// for d in data {
	//    xmin = std::min(d[0], min);
	//    ymin = std::min(d[1], ymin);
	//    zmin = std::min(d[2], zmin);
	// }

	m_isComplete = true;

	return true;
}

void QuadStrip::vertexAttrib(VertexElementSemantic semantic, void* data) {
	std::map<VertexElementSemantic, uint32_t>::const_iterator it = m_sizePerSemantic.find(semantic);
	assert(it != m_sizePerSemantic.end());
	if (it == m_sizePerSemantic.end()) {
		return;
	}

	VertexElement* ve = m_vf->getElementBySemantic(semantic);
	if (!ve) {
		return;
	}

	// parameter data points to a single vertex attribute
	uint index = m_sizePerSemantic[semantic];
	assert(index < m_numVertices);
	if (index < m_numVertices) {
		ve->updateDataAtIndex(index, data);
	}

	++m_sizePerSemantic[semantic];

}

void QuadStrip::vertexAttribArray(VertexElementSemantic semantic, void* data, uint32_t offset, uint32_t count) {

	std::map<VertexElementSemantic, uint32_t>::const_iterator it = m_sizePerSemantic.find(semantic);
	assert(it != m_sizePerSemantic.end());
	if (it == m_sizePerSemantic.end()) {
		return;
	}

	VertexElement* ve = m_vf->getElementBySemantic(semantic);
	if (!ve) {
		return;
	}

	// a zero size implies all vertices
	if (!count) {
		count = m_numVertices;
	}
	assert(offset < m_numVertices);
	assert(count <= (m_numVertices - offset));
	if (offset < m_numVertices && count <= (m_numVertices - offset)) {
		ve->m_vbo->updateSubData(offset, count, data);
		m_sizePerSemantic[semantic] += count;
	}

}

void QuadStrip::renderGeometry() {
	if (m_isComplete && m_ibo) {
//		m_ibo->drawPrimitives();
		glDrawArrays(GL_QUAD_STRIP, 0, m_numVertices);
	}
}
