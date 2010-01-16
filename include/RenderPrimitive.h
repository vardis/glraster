/*
 * RenderPrimitive.h
 *
 *  Created on: Jan 11, 2010
 *      Author: giorgos
 */

#ifndef RENDERPRIMITIVE_H_
#define RENDERPRIMITIVE_H_

#include "Renderable.h"
#include "VertexFormat.h"
#include "VertexIndexBuffer.h"

struct QuadsPrimitiveType {
	GLenum getGLDrawType() const {
		return GL_QUADS;
	}

	uint8_t numVerticesForPrimitives(uint32_t numPrimitives) const {
		return 4 * numPrimitives;
	}
	bool hasIndices() const {
		return false;
	}
};

struct QuadStripPrimitiveType {

	GLenum getGLDrawType() const {
		return GL_QUAD_STRIP;
	}

	uint8_t numVerticesForPrimitives(uint32_t numPrimitives) const {
		return 2 * numPrimitives + 2;
	}
	bool hasIndices() const {
		return false;
	}
};

#ifdef USE_EXPORT_KEYWORD
export
#endif
template<typename T>
class RenderPrimitive: public Renderable {

private:
	T m_primitiveType;
	uint32_t m_numPrimitives;
	uint32_t m_numVertices;
	uint32_t m_numIndices;
	bool m_hasIndices;
	VertexFormatPtr m_vf;

private:
	bool m_isSpecified;
	std::map<VertexElementSemantic, uint32_t> m_sizePerSemantic;

public:
	RenderPrimitive();
	virtual ~RenderPrimitive();

	virtual void renderGeometry();
	void specifyVertexFormat(const VertexFormat & vf);
	void specifyVertexElement(VertexElementSemantic semantic, VertexElementFormat format);
	void beginGeometry(uint32_t numPrimitives);
	bool endGeometry();
	void vertexAttrib(VertexElementSemantic semantic, void *data);
	void vertexAttribArray(VertexElementSemantic semantic, void *data, uint32_t offset = 0, uint32_t count = 0);

	bool hasIndices() const {
		return m_hasIndices;
	}

	bool isSpecified() const {
		return m_isSpecified;
	}

	uint32_t getNumIndices() const {
		return m_numIndices;
	}

	uint32_t getNumPrimitives() const {
		return m_numPrimitives;
	}

	uint32_t getNumVertices() const {
		return m_numVertices;
	}

	T getPrimitiveType() const {
		return m_primitiveType;
	}

	std::map<VertexElementSemantic, uint32_t> getSizePerSemantic() const {
		return m_sizePerSemantic;
	}

	VertexFormatPtr getVf() const {
		return m_vf;
	}

	void setHasIndices(bool m_hasIndices) {
		this->m_hasIndices = m_hasIndices;
	}

	void setIsSpecified(bool m_isSpecified) {
		this->m_isSpecified = m_isSpecified;
	}

	void setNumIndices(uint32_t m_numIndices) {
		this->m_numIndices = m_numIndices;
	}

	void setNumPrimitives(uint32_t m_numPrimitives) {
		this->m_numPrimitives = m_numPrimitives;
	}

	void setNumVertices(uint32_t m_numVertices) {
		this->m_numVertices = m_numVertices;
	}

	void setPrimitiveType(T m_primitiveType) {
		this->m_primitiveType = m_primitiveType;
	}

	void setSizePerSemantic(std::map<VertexElementSemantic, uint32_t> m_sizePerSemantic) {
		this->m_sizePerSemantic = m_sizePerSemantic;
	}

	void setVf(VertexFormatPtr m_vf) {
		this->m_vf = m_vf;
	}

};

#endif /* RENDERPRIMITIVE_H_ */
