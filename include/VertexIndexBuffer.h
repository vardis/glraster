/*
 * VertexIndexBuffer.h
 *
 *  Created on: Dec 26, 2009
 *      Author: giorgos
 */

#ifndef VERTEXINDEXBUFFER_H_
#define VERTEXINDEXBUFFER_H_

class VertexIndexBuffer {
public:
	// number of primitives, type of primitives
	GLenum   m_primitiveType;
	GLuint   m_iboID;
	uint32_t m_numIndices;
	GLenum   m_usageHint;

public:
	VertexIndexBuffer(GLenum primitiveType);
	virtual ~VertexIndexBuffer();

	// bind, unbind, drawPrimitives(instances_count)

	void setData(uint32_t* data, uint32_t numIndices);
	void setData(IndexArrayPtr data, uint32_t numIndices);

	void allocate(uint32_t numIndices);
	void deallocate();

	void bind();
	void unbind();

	void drawPrimitives(uint16_t numInstances = 1);

	uint32_t* mapData(GLenum accessType, uint32_t offset, uint32_t length);
	void unmapData();
};
typedef shared_ptr<VertexIndexBuffer> VertexIndexBufferPtr;

#endif /* VERTEXINDEXBUFFER_H_ */
