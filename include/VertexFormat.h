/*
 * VertexFormat.h
 *
 *  Created on: Nov 27, 2009
 *      Author: giorgos
 */

#ifndef VERTEXFORMAT_H_
#define VERTEXFORMAT_H_

#define MAX_VERTEX_ELEMENTS 32

typedef enum VertexElementSemantic {
	Vertex_Pos = 1,
	Vertex_Normal = 2,
	Vertex_BiNormal = 4,
	Vertex_Tangent = 8,
	Vertex_Weights = 16,
	Vertex_Color = 32,
	Vertex_TexCoord0 = 64,
	Vertex_TexCoord1 = 128,
	Vertex_TexCoord2 = 256,
	Vertex_TexCoord3 = 512,
	Vertex_TexCoord4 = 1024,
	Vertex_TexCoord5 = 2048,
	Vertex_TexCoord6 = 4096,
	Vertex_TexCoord7 = 8192
} VertexElementSemantic;

typedef enum VertexElementFormat {
	VertexFormat_FLOAT1,
	VertexFormat_FLOAT2,
	VertexFormat_FLOAT3,
	VertexFormat_FLOAT4,
	VertexFormat_SHORT1,
	VertexFormat_SHORT2,
	VertexFormat_SHORT3,
	VertexFormat_SHORT4,
	VertexFormat_UBYTE3,
	VertexFormat_UBYTE4,
} VertexElementFormat;

#include "VertexElementBuffer.h"

typedef struct VertexElement {

	VertexElementSemantic m_semantic;
	VertexElementFormat m_format;
	uint16_t m_size;
	uint32_t m_offset;
	//	void* m_data;
	VertexElementBufferPtr m_vbo;

	VertexElement(VertexElementSemantic semantic, VertexElementFormat format, uint32_t offset, void* data) :
		m_semantic(semantic), m_format(format), m_size(getFormatSize(format)), m_offset(offset), /*m_data(data),*/
				m_vbo() {
		m_vbo = VertexElementBufferPtr(new VertexElementBuffer(this));
	}
	~VertexElement() {
		//		_deleteData();
	}

	VertexElement& operator=(const VertexElement& rhs) {
		m_semantic = rhs.m_semantic;
		m_format = rhs.m_format;
		m_offset = rhs.m_offset;
		m_size = rhs.m_size;
		//		m_data     = rhs.m_data;
		m_vbo = rhs.m_vbo;
		return *this;
	}

	static uint8_t getFormatSize(VertexElementFormat format) {
		switch (format) {
		case VertexFormat_FLOAT1:
			return sizeof(float);
		case VertexFormat_FLOAT2:
			return 2 * sizeof(float);
		case VertexFormat_FLOAT3:
			return 3 * sizeof(float);
		case VertexFormat_FLOAT4:
			return 4 * sizeof(float);
		case VertexFormat_SHORT1:
			return sizeof(short);
		case VertexFormat_SHORT2:
			return 2 * sizeof(short);
		case VertexFormat_SHORT3:
			return 3 * sizeof(short);
		case VertexFormat_SHORT4:
			return 4 * sizeof(short);
		case VertexFormat_UBYTE3:
			return 3 * sizeof(uint8_t);
		case VertexFormat_UBYTE4:
			return 4 * sizeof(uint8_t);
		default:
			// TODO: error handling
			return 0;
		}
	}
	static GLenum getFormatType(VertexElementFormat format) {
		switch (format) {
		case VertexFormat_FLOAT1:
		case VertexFormat_FLOAT2:
		case VertexFormat_FLOAT3:
		case VertexFormat_FLOAT4:
			return GL_FLOAT;
		case VertexFormat_SHORT1:
		case VertexFormat_SHORT2:
		case VertexFormat_SHORT3:
		case VertexFormat_SHORT4:
			return GL_SHORT;
		case VertexFormat_UBYTE3:
		case VertexFormat_UBYTE4:
			return GL_UNSIGNED_BYTE;
		default:
			// TODO: error handling
			return 0;
		}
	}

	static uint8_t getFormatElementCount(VertexElementFormat format) {
		switch (format) {
		case VertexFormat_FLOAT1:
		case VertexFormat_SHORT1:
			return 1;
		case VertexFormat_FLOAT2:
		case VertexFormat_SHORT2:
			return 2;
		case VertexFormat_FLOAT3:
		case VertexFormat_SHORT3:
		case VertexFormat_UBYTE3:
			return 3;
		case VertexFormat_FLOAT4:
		case VertexFormat_SHORT4:
		case VertexFormat_UBYTE4:
			return 4;
		default:
			// TODO: error handling
			return 0;
		}
	}

	/*
	 void updateSubData(uint32_t offset, uint32_t count, void* data) {
	 float* fdst, *fsrc;
	 short* udst, *usrc;
	 uint8_t* bdst, *bsrc;

	 if (m_data) {
	 switch (m_format) {
	 case VertexFormat_FLOAT1:
	 case VertexFormat_FLOAT2:
	 case VertexFormat_FLOAT3:
	 case VertexFormat_FLOAT4:
	 fdst = (float*) m_data;
	 fsrc = (float*) data;
	 memcpy(fdst, fsrc, getFormatSize(m_format)*count);
	 break;
	 case VertexFormat_SHORT1:
	 case VertexFormat_SHORT2:
	 case VertexFormat_SHORT3:
	 case VertexFormat_SHORT4:
	 udst = (short*) m_data;
	 usrc = (short*) data;
	 memcpy(udst, usrc, getFormatSize(m_format)*count);
	 break;
	 case VertexFormat_UBYTE3:
	 case VertexFormat_UBYTE4:
	 bdst = (uint8_t*) m_data;
	 bsrc = (uint8_t*) data;
	 memcpy(bdst, bsrc, getFormatSize(m_format)*count);
	 break;
	 default:
	 //TODO: error reporting
	 break;
	 }
	 }
	 }
	 */
	void updateDataAtIndex(uint32_t index, void* data) {
		float* fdst, *fsrc;
		short* udst, *usrc;
		uint8_t* bdst, *bsrc;
		void* mapped = m_vbo->mapData();
		if (mapped) {
			switch (m_format) {
			case VertexFormat_FLOAT1:
			case VertexFormat_FLOAT2:
			case VertexFormat_FLOAT3:
			case VertexFormat_FLOAT4:
				fdst = (float*) mapped;
				fdst += (index*getFormatElementCount(m_format));
				fsrc = (float*) data;
				for (int i = 0; i < getFormatElementCount(m_format); i++) {
					*fdst++ = *fsrc++;
				}
				break;
			case VertexFormat_SHORT1:
			case VertexFormat_SHORT2:
			case VertexFormat_SHORT3:
			case VertexFormat_SHORT4:
				udst = (short*) mapped;
				udst += (index*getFormatElementCount(m_format));
				usrc = (short*) data;
				for (int i = 0; i < getFormatElementCount(m_format); i++) {
					*udst++ = *usrc++;
				}
				break;
			case VertexFormat_UBYTE3:
			case VertexFormat_UBYTE4:
				bdst = (uint8_t*) mapped;
				bdst += (index*getFormatElementCount(m_format));
				bsrc = (uint8_t*) data;
				for (int i = 0; i < getFormatElementCount(m_format); i++) {
					*bdst++ = *bsrc++;
				}
				break;
			default:
				//TODO: error reporting
				break;
			}
			m_vbo->unmapData();
		}
	}

	//	void* _allocateArrayForElement(uint32_t count) {
	//		m_vbo->allocate(count);
	//		return m_vbo->mapData();
	//		switch (m_format) {
	//		case VertexFormat_FLOAT1:
	//		case VertexFormat_FLOAT2:
	//		case VertexFormat_FLOAT3:
	//		case VertexFormat_FLOAT4:
	//			m_data = new float[count * VertexElement::getFormatElementCount(m_format)];
	//			break;
	//		case VertexFormat_SHORT1:
	//		case VertexFormat_SHORT2:
	//		case VertexFormat_SHORT3:
	//		case VertexFormat_SHORT4:
	//			m_data = new short[count * VertexElement::getFormatElementCount(m_format)];
	//			break;
	//		case VertexFormat_UBYTE3:
	//		case VertexFormat_UBYTE4:
	//			m_data = new uint8_t[count * VertexElement::getFormatElementCount(m_format)];
	//			break;
	//		default:
	//			m_data = 0;
	//		}
	//		return m_data;
	//	}
	/*
	 void _deleteData() {
	 if (m_data) {
	 switch (m_format) {
	 case VertexFormat_FLOAT1:
	 case VertexFormat_FLOAT2:
	 case VertexFormat_FLOAT3:
	 case VertexFormat_FLOAT4:
	 delete[] (float*) m_data;

	 case VertexFormat_SHORT1:
	 case VertexFormat_SHORT2:
	 case VertexFormat_SHORT3:
	 case VertexFormat_SHORT4:
	 delete[] (short*) m_data;

	 case VertexFormat_UBYTE3:
	 case VertexFormat_UBYTE4:
	 delete[] (uint8_t*) m_data;
	 default:
	 //TODO:error reporting
	 return;
	 }
	 }
	 }
	 */

} VertexElement;
typedef shared_ptr<VertexElement> VertexElementPtr;

class VertexFormat {
public:
	VertexFormat();
	~VertexFormat();

	VertexFormat& operator=(const VertexFormat& rhs);
	VertexElement* addElement(VertexElement* ve);
	VertexElement* addElement(VertexElementSemantic semantic, VertexElementFormat format, uint32_t offset, void* data);
	VertexElement* addElement(VertexElementSemantic semantic, VertexElementFormat format);
	VertexElement* getElementByIndex(uint8_t numElement);
	VertexElement* getElementBySemantic(VertexElementSemantic semantic);
	uint8_t getNumElements() const;

	void bindData() {
		for (uint8_t i = 0; i < m_numElements; i++) {
			VertexElement* ve = getElementByIndex(i);
			ve->m_vbo->uploadData();
		}
	}

	void printData();
private:
	uint8_t m_numElements;
	VertexElementPtr m_elements[MAX_VERTEX_ELEMENTS];
};
typedef shared_ptr<VertexFormat> VertexFormatPtr;

#endif /* VERTEXFORMAT_H_ */
