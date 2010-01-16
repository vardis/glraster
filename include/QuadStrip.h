/*
 * QuadStrip.h
 *
 *  Created on: Jan 3, 2010
 *      Author: giorgos
 */

#ifndef QUADSTRIP_H_
#define QUADSTRIP_H_

#include "Renderable.h"
#include "VertexFormat.h"
#include "VertexIndexBuffer.h"

/**
 * Provides efficient rendering of a quad-strip by backing up the rendering of the strips with a VBO.
 * For every quad vertex the following attributes are supported:
 *   -vertex position
 *   -uv coords
 *   -color
 *
 *   TODO: Could this be turned into a generic GeometricPrimitive class which will be specialized
 *   through a GLenum geometryType field?
 */
class QuadStrip : public Renderable {
public:
	uint32_t             m_numQuads;
	uint32_t             m_numVertices;
	VertexFormatPtr      m_vf;
	VertexIndexBufferPtr m_ibo;
	uint32_t             m_currentQuad;

	/** indicates if all data for rendering the quad strip have been specified by the user */
	bool                 m_isComplete;

private:
	// keeps track of the number of vertex elements' semantics that have been specified by the user
	std::map<VertexElementSemantic, uint32_t> m_sizePerSemantic;

public:
	QuadStrip();
	virtual ~QuadStrip();

	virtual void renderGeometry();

	void specifyVertexFormat(const VertexFormat& vf);
	void specifyVertexElement(VertexElementSemantic semantic, VertexElementFormat format);

	void beginQuadStrip(uint32_t numQuads);
	/**
	 * @return true if the quad strip was successfully builded, false if the data were incomplete
	 */
	bool endQuadStrip();

	/**
	 * Specifies data for the given vertex semantic. You don't pass an index to specify the target vertex, instead
	 * the last specified vertex is being tracked automatically separately for each semantic.
	 * E.g.
	 *   vertexAttrib(Vertex_Pos, ...);    // A
	 *   vertexAttrib(Vertex_Pos, ...);    // B
	 *
	 * Statement A would provide the position of vertex 0 and B the position of vertex 1. While a later statement
	 * for a different semantic would target a different vertex, e.g.:
	 *   vertexAttrib(Vertex_Normal, ...); // C
	 *
	 * Statement C would provide the normal of vertex 0
	 *
	 * @param semantic the vertex semantic
	 * @param data pointer to data
	 */
	void vertexAttrib(VertexElementSemantic semantic, void* data);

	/**
	 * Specifies data for the attributes of a continuous range of vertices.
	 *
	 * @param semantic the vertex semantic
	 * @param data pointer to data
	 * @param offset the index of the first vertex being specified
	 * @param count the number of vertices
	 */
	void vertexAttribArray(VertexElementSemantic semantic, void* data, uint32_t offset = 0, uint32_t count = 0);
};

#endif /* QUADSTRIP_H_ */
