/*
 * RenderPrimitive.cpp
 *
 *  Created on: Jan 11, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "RenderPrimitive.h"

template<typename T>
RenderPrimitive<T>::RenderPrimitive() :
	Renderable(), m_primitiveType(), m_numPrimitives(0), m_numVertices(0), m_numIndices(0), m_hasIndices(false),
			m_vf(), m_isSpecified(false), m_hasBindedOnce(false) {
	glGenVertexArrays(1, &m_vao);
}

template<typename T>
RenderPrimitive<T>::~RenderPrimitive() {
	glDeleteVertexArrays(1, &m_vao);
}

template<typename T>
void RenderPrimitive<T>::specifyVertexFormat(VertexFormatPtr vf) {
	m_hasBindedOnce = false;
	//	m_vf.reset(new VertexFormat());
	//	(*m_vf) = vf;
	m_vf = vf;

	// reset counters for each vertex element
	for (uint8_t i = 0; i < m_vf->getNumElements(); i++) {
		VertexAttribute* ve = m_vf->getAttributeByIndex(i);
		m_sizePerSemantic[ve->m_semantic] = 0;
	}
	m_isSpecified = false;
}

template<typename T>
void RenderPrimitive<T>::beginGeometry(uint32_t numPrimitives) {
	assert(m_vf);

	m_hasBindedOnce = false;

	// reset counters for each vertex element
	for (uint8_t i = 0; i < m_vf->getNumElements(); i++) {
		VertexAttribute* ve = m_vf->getAttributeByIndex(i);
		m_sizePerSemantic[ve->m_semantic] = 0;
	}
	m_isSpecified = false;

	m_numPrimitives = numPrimitives;
	m_numVertices = m_primitiveType.numVerticesForPrimitives(m_numPrimitives);
	for (uint8_t i = 0; i < m_vf->getNumElements(); i++) {
		VertexAttribute* ve = m_vf->getAttributeByIndex(i);
		ve->m_vbo->allocate(m_numVertices);
	}
}

template<typename T>
bool RenderPrimitive<T>::endGeometry() {
	// verify that the position semantic has been specified
	std::map<VertexAttributeSemantic, uint32_t>::const_iterator it = m_sizePerSemantic.find(Vertex_Pos);
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
		VertexAttribute* ve = m_vf->getAttributeByIndex(i);
		ve->m_vbo->uploadData();
	}

	// update bounding box
	m_bounds = AABB<float>::EmptyAABB();
	VertexAttribute* ve = m_vf->getAttributeBySemantic(Vertex_Pos);
	//TODO: !IMPORTANT! complete this...
	// data = vbo->mapData()
	// for d in data {
	//    xmin = std::min(d[0], min);
	//    ymin = std::min(d[1], ymin);
	//    zmin = std::min(d[2], zmin);
	// }

	m_isSpecified = true;

	return true;
}

template<typename T>
void RenderPrimitive<T>::vertexAttrib(VertexAttributeSemantic semantic, void* data) {
	assert(m_vf);
	std::map<VertexAttributeSemantic, uint32_t>::const_iterator it = m_sizePerSemantic.find(semantic);
	assert(it != m_sizePerSemantic.end());
	if (it == m_sizePerSemantic.end()) {
		SAFE_THROW(GLException(E_BADARG, "No such semantic"));
	}

	VertexAttribute* ve = m_vf->getAttributeBySemantic(semantic);
	assert(ve);
	if (!ve) {
		SAFE_THROW(GLException(E_BADSTATE, "No VertexAttribute for semantic"));
	}

	// parameter data points to a single vertex attribute
	uint index = m_sizePerSemantic[semantic];
	assert(index < m_numVertices);
	if (index < m_numVertices) {
		ve->updateDataAtIndex(index, data);
		++m_sizePerSemantic[semantic];
	} else {
		SAFE_THROW(GLException(E_BADOP, "Too many values for a vertex attribute"));
	}
}

template<typename T>
void RenderPrimitive<T>::vertexAttribArray(VertexAttributeSemantic semantic, void* data, uint32_t offset,
		uint32_t count) {
	assert(m_vf);
	std::map<VertexAttributeSemantic, uint32_t>::const_iterator it = m_sizePerSemantic.find(semantic);
	assert(it != m_sizePerSemantic.end());
	if (it == m_sizePerSemantic.end()) {
		SAFE_THROW(GLException(E_BADARG, "No such semantic"));
	}

	VertexAttribute* ve = m_vf->getAttributeBySemantic(semantic);
	assert(ve);
	if (!ve) {
		SAFE_THROW(GLException(E_BADSTATE, "No VertexAttribute for semantic"));
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
	} else {
		SAFE_THROW(GLException(E_BADOP, "Too many values for a vertex attribute"));
	}
}

template<typename T>
void RenderPrimitive<T>::renderGeometry() {
	assert(m_isSpecified);
	if (m_isSpecified) {
		glBindVertexArray(m_vao);
		if (!m_hasBindedOnce) {
			m_vf->bindData();
			m_hasBindedOnce = true;
		}
		glDrawArrays(m_primitiveType.getGLDrawType(), 0, m_numVertices);
		glBindVertexArray(0);
	}
}

// Classes templates instantiations
#ifndef USE_EXPORT_KEYWORD
template class RenderPrimitive<QuadStripPrimitiveType> ;
template class RenderPrimitive<QuadsPrimitiveType> ;
template class RenderPrimitive<TrianglesPrimitiveType> ;
#endif
