#include "GLTutor.h"
#include "Mesh.h"

Mesh::Mesh() :
	m_vertexFormat(), m_numVertices(0), m_vertexDataUpdated(false), m_indices(0), m_numIndices(0),
	m_indexDataUpdated(false), m_material() {
}

Mesh::~Mesh(void) {
}

void Mesh::updateVertexData(VertexFormat* vf, uint32_t _count) {
	m_vertexFormat = VertexFormatPtr(vf);
	m_numVertices = _count;
	m_vertexDataUpdated = true;
}

void Mesh::updateIndexData(IndexArrayPtr _idx, uint32_t _count) {
	m_indices = _idx;
	m_numIndices = _count;
	m_indexDataUpdated = true;
}

void Mesh::updateIndexData(uint32_t* _idx, uint32_t _count, bool _copyData) {
	if (_copyData && m_numIndices == _count) {
		memcpy(m_indices.get(), _idx, _count);
	} else if (_copyData) {
		m_indices = IndexArrayPtr(new uint32_t[_count]);
		m_numIndices = _count;
		memcpy(m_indices.get(), _idx, _count);
	} else {
		m_indices = IndexArrayPtr(_idx);
		m_numIndices = _count;
	}
	m_indexDataUpdated = true;
}

