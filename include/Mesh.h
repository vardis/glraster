#pragma once

#include "AABB.h"
#include "VertexFormat.h"
#include "VertexIndexBuffer.h"
#include "Material.h"

class Mesh {

protected:
	VertexFormatPtr m_vertexFormat;
	uint32_t m_numVertices;

	bool m_vertexDataUpdated;
	bool m_indexDataUpdated;

	VertexIndexBufferPtr m_ibo;

	MaterialPtr m_material;

	AABB<float> m_bounds;

public:
	Mesh();
	virtual ~Mesh(void);

	void updateVertexData(VertexFormat* vf, uint32_t _count);

	void updateIndexData(IndexArrayPtr _idx, uint32_t _count);

	VertexFormatPtr getVertexFormat() {
		return m_vertexFormat;//.get();
	}

	uint32_t getNumIndices() const {
		return m_ibo->m_numIndices;
	}

	void setNumVertices(uint32_t numVertices) {
		this->m_numVertices = numVertices;
	}

	uint32_t getNumVertices() const {
		return m_numVertices;
	}

	bool isVertexDataUpdated() const {
		return m_vertexDataUpdated;
	}
	void setVertexDataUpdated(bool v) {
		m_vertexDataUpdated = v;
	}

	bool isIndexDataUpdated() const {
		return m_indexDataUpdated;
	}
	void setIndexDataUpdated(bool v) {
		m_indexDataUpdated = v;
	}

	VertexIndexBufferPtr getIbo() const {
		return m_ibo;
	}

	void setMaterial(MaterialPtr material) {
		m_material = material;
	}
	MaterialPtr getMaterial() const {
		return m_material;
	}

	AABB<float> getBounds() {
		return m_bounds;
	}

	void setBounds(const AABB<float> bounds) {
		m_bounds = bounds;
	}

private:
	Mesh(const Mesh& _rhs);
	Mesh & operator=(const Mesh& _rhs);

protected:
	void _updateBounds();
	void _updateVertexBuffer();
	void _updateIndicesBuffer();
};

typedef shared_ptr<Mesh> MeshPtr;

