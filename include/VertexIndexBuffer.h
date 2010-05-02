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

private:
	bool     m_isMapped;


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

	uint32_t* mapData(GLenum accessType = GL_READ_WRITE);
	uint32_t* mapSubData(GLbitfield accessType = GL_MAP_WRITE_BIT, uint32_t offset = 0, uint32_t count = 0);
	void unmapData();

	void printData();
};
typedef shared_ptr<VertexIndexBuffer> VertexIndexBufferPtr;

#endif /* VERTEXINDEXBUFFER_H_ */
