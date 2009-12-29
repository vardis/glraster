/*
 * VertexElementBuffer.h
 *
 *  Created on: Dec 26, 2009
 *      Author: giorgos
 */

#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

//#include "VertexFormat.h"

class VertexElement;

class VertexElementBuffer {
public:
	GLuint         m_vboID;
	VertexElement* m_ve;
	uint32_t       m_numElements;
	GLenum         m_usageHint;

public:
	VertexElementBuffer(VertexElement* ve);
	virtual ~VertexElementBuffer();

	/** size is in bytes */
	void setData(void* data, uint32_t numElements);

	void allocate(uint32_t numElements);
	void deallocate();

	void bind();
	void unbind();

	void uploadData();
};
typedef shared_ptr<VertexElementBuffer> VertexElementBufferPtr;

#endif /* VERTEXBUFFER_H_ */
