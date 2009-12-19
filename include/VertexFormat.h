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
	Vertex_Pos,
	Vertex_Normal,
	Vertex_BiNormal,
	Vertex_Tangent,
	Vertex_Weights,
	Vertex_Color,
	Vertex_TexCoord0,
	Vertex_TexCoord1,
	Vertex_TexCoord2,
	Vertex_TexCoord3,
	Vertex_TexCoord4,
	Vertex_TexCoord5,
	Vertex_TexCoord6,
	Vertex_TexCoord7
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

struct VertexElement {
	VertexElement() :
		m_semantic(Vertex_Pos), m_format(VertexFormat_FLOAT3), m_size(0), m_offset(0), m_data(0), m_vboID(0) {
	}
	VertexElement(VertexElementSemantic semantic, VertexElementFormat format, uint32_t offset, void* data) :
		m_semantic(semantic), m_format(format), m_size(getFormatSize(format)), m_offset(offset), m_data(data), m_vboID(
				0) {

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
	VertexElementSemantic m_semantic;
	VertexElementFormat m_format;
	uint16_t m_size;
	uint32_t m_offset;
	void* m_data;
	GLuint m_vboID;
};
typedef shared_ptr<VertexElement> VertexElementPtr;

class VertexFormat {
public:
	VertexFormat();
	~VertexFormat();

	VertexElement* addElement(VertexElement* ve);
	VertexElement* addElement(VertexElementSemantic semantic, VertexElementFormat format, uint32_t offset, void* data);
	VertexElement* getElementByIndex(uint8_t numElement);
	VertexElement* getElementBySemantic(VertexElementSemantic semantic);
	uint8_t getNumElements() const;

private:
	uint8_t m_numElements;
	VertexElementPtr m_elements[MAX_VERTEX_ELEMENTS];
};
typedef shared_ptr<VertexFormat> VertexFormatPtr;

#endif /* VERTEXFORMAT_H_ */
