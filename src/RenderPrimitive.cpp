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
	m_primitiveType(), m_numPrimitives(0), m_numVertices(0), m_numIndices(0), m_hasIndices(false), m_vf(),
			m_isSpecified(false) {

}

template<typename T>
RenderPrimitive<T>::~RenderPrimitive() {

}

template<typename T>
void RenderPrimitive<T>::specifyVertexFormat(const VertexFormat& vf) {
	m_vf.reset(new VertexFormat());
	(*m_vf) = vf;

	// reset counters for each vertex element
	for (uint8_t i = 0; i < m_vf->getNumElements(); i++) {
		VertexElement* ve = m_vf->getElementByIndex(i);
		m_sizePerSemantic[ve->m_semantic] = 0;
	}
	m_isSpecified = false;
}

template<typename T>
void RenderPrimitive<T>::beginGeometry(uint32_t numPrimitives) {
	assert(m_vf);
	m_numPrimitives = numPrimitives;
	m_numVertices = m_primitiveType.numVerticesForPrimitives(m_numPrimitives);
	for (uint8_t i = 0; i < m_vf->getNumElements(); i++) {
		VertexElement* ve = m_vf->getElementByIndex(i);
		ve->m_vbo->allocate(m_numVertices);
	}
}

template<typename T>
bool RenderPrimitive<T>::endGeometry() {
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

	m_isSpecified = true;

	return true;
}

template<typename T>
void RenderPrimitive<T>::vertexAttrib(VertexElementSemantic semantic, void* data) {
	assert(m_vf);
	std::map<VertexElementSemantic, uint32_t>::const_iterator it = m_sizePerSemantic.find(semantic);
	assert(it != m_sizePerSemantic.end());
	if (it == m_sizePerSemantic.end()) {
		throw GLException(E_BADARG);
	}

	VertexElement* ve = m_vf->getElementBySemantic(semantic);
	assert(ve);
	if (!ve) {
		throw GLException(E_BADSTATE);
	}

	// parameter data points to a single vertex attribute
	uint index = m_sizePerSemantic[semantic];
	assert(index < m_numVertices);
	if (index < m_numVertices) {
		ve->updateDataAtIndex(index, data);
		++m_sizePerSemantic[semantic];
	} else {
		throw GLException(E_BADOP);
	}
}

template<typename T>
void RenderPrimitive<T>::vertexAttribArray(VertexElementSemantic semantic, void* data, uint32_t offset, uint32_t count) {
	assert(m_vf);
	std::map<VertexElementSemantic, uint32_t>::const_iterator it = m_sizePerSemantic.find(semantic);
	assert(it != m_sizePerSemantic.end());
	if (it == m_sizePerSemantic.end()) {
		throw GLException(E_BADARG);
	}

	VertexElement* ve = m_vf->getElementBySemantic(semantic);
	assert(ve);
	if (!ve) {
		throw GLException(E_BADSTATE);
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
		throw GLException(E_BADOP);
	}
}

template<typename T>
void RenderPrimitive<T>::renderGeometry() {
	assert(m_isSpecified);
	if (m_isSpecified) {
		m_vf->bindData();
		glDrawArrays(m_primitiveType.getGLDrawType(), 0, m_numVertices);
	}
}

// Classes templates instantiations
#ifndef USE_EXPORT_KEYWORD
template class RenderPrimitive<QuadStripPrimitiveType> ;
template class RenderPrimitive<QuadsPrimitiveType> ;
#endif
