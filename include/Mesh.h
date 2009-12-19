#pragma once

#include "VertexFormat.h"
#include "Material.h"

typedef boost::shared_ptr<uint32_t> IndexPtr;
typedef boost::shared_array<uint32_t> IndexArrayPtr;

class Mesh {

protected:
	VertexFormatPtr m_vertexFormat;
	uint32_t m_numVertices;
	bool m_vertexDataUpdated;

	IndexArrayPtr m_indices;
	uint32_t m_numIndices;
	bool m_indexDataUpdated;

	MaterialPtr m_material;

public:
	Mesh();
	virtual ~Mesh(void);

	void updateVertexData(VertexFormat* vf, uint32_t _count);

	void updateIndexData(IndexArrayPtr _idx, uint32_t _count);

	void updateIndexData(uint32_t* _idx, uint32_t _count, bool _copyData = false);

	VertexFormat* getVertexFormat() {
		return m_vertexFormat.get();
	}

	uint32_t* getIndices() {
		return m_indices.get();
	}

	void setNumIndices(uint32_t numIndices) {
		this->m_numIndices = numIndices;
	}

	uint32_t getNumIndices() const {
		return m_numIndices;
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

	void setMaterial(MaterialPtr material) {
		m_material = material;
	}
	MaterialPtr getMaterial() const {
		return m_material;
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

