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

/**
 * First allocate data for the buffer:
 *   vbo.allocate(1024);
 *
 * Then set data through an array:
 *   vbo.setData(array, 1024);
 *
 * Portions of the data can be update with:
 *   vbo.updateSubData(100, 200, newData);
 *
 * To bind the data to the respective vertex attribute:
 *   vbo.uploadData();
 *
 * To map the buffer's data on the CPU side for reading/writing:
 *   vbo.mapData(GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT, 0, 1024);
 *
 * When done, you can unmap with:
 *   vbo.unmapData();
 *
 * When you are done with this buffer and want to explicitly clean up:
 *   vbo.deallocate();
 */
class VertexElementBuffer {
public:
	GLuint         m_vboID;
	VertexElement* m_ve;
	uint32_t       m_numElements;
	GLenum         m_usageHint;

private:
	void*          m_mapped;

public:
	VertexElementBuffer(VertexElement* ve);
	virtual ~VertexElementBuffer();

	void setData(void* data, uint32_t numElements);

	void updateSubData(uint32_t offset, uint32_t count, void* data);

	void allocate(uint32_t numElements);
	void deallocate();

	void bind();
	void unbind();
	void printData();
	void uploadData();

	bool isMapped() const {
		return m_mapped != 0;
	}

	void* mapData(GLenum accessType = GL_READ_WRITE);
	void* mapSubData(GLbitfield accessType = GL_MAP_WRITE_BIT, uint32_t offset = 0, uint32_t count = 0);
	void unmapData();
};
typedef shared_ptr<VertexElementBuffer> VertexElementBufferPtr;

#endif /* VERTEXBUFFER_H_ */
