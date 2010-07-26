/*
 * VertexFormat.h
 *
 *  Created on: Nov 27, 2009
 *      Author: giorgos
 */

#ifndef VERTEXFORMAT_H_
#define VERTEXFORMAT_H_

#define MAX_VERTEX_ELEMENTS 32

typedef enum VertexAttributeSemantic {
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
} VertexAttributeSemantic;

typedef enum VertexAttributeFormat {
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
} VertexAttributeFormat;

#include "VertexAttributeBuffer.h"

typedef struct VertexAttribute {

	VertexAttributeSemantic m_semantic;
	VertexAttributeFormat m_format;
	uint16_t m_size;
	uint32_t m_offset;
	VertexAttributeBufferPtr m_vbo;

	VertexAttribute(VertexAttributeSemantic semantic, VertexAttributeFormat format, uint32_t offset, void* data) :
		m_semantic(semantic), m_format(format), m_size(getFormatSize(format)), m_offset(offset), m_vbo() {
		m_vbo = VertexAttributeBufferPtr(new VertexAttributeBuffer(this));
	}
	~VertexAttribute() {

	}

	VertexAttribute& operator=(const VertexAttribute& rhs) {
		m_semantic = rhs.m_semantic;
		m_format = rhs.m_format;
		m_offset = rhs.m_offset;
		m_size = rhs.m_size;
		m_vbo = rhs.m_vbo;
		return *this;
	}

	static uint8_t getFormatSize(VertexAttributeFormat format) {
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
	static GLenum getFormatType(VertexAttributeFormat format) {
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

	static uint8_t getFormatElementCount(VertexAttributeFormat format) {
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
				fdst += (index * getFormatElementCount(m_format));
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
				udst += (index * getFormatElementCount(m_format));
				usrc = (short*) data;
				for (int i = 0; i < getFormatElementCount(m_format); i++) {
					*udst++ = *usrc++;
				}
				break;
			case VertexFormat_UBYTE3:
			case VertexFormat_UBYTE4:
				bdst = (uint8_t*) mapped;
				bdst += (index * getFormatElementCount(m_format));
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

} VertexAttribute;
typedef shared_ptr<VertexAttribute> VertexAttributePtr;

enum VertexFormatTemplates {
	/** x, y, z position */
	VF_V3 = 1,

	/** x, y position */
	VF_V2 = 2,

	/** RGBA color */
	VF_C4 = 4,

	/** normal */
	VF_N3 = 8,

	/** u, v coordinates */
	VF_T2 = 16,

	/** x, y, z position, uv */
	VF_V3_T2 = VF_V3 | VF_T2,

	/** x, y, z position, RGBA color */
	VF_V3_C4 = VF_V3 | VF_C4,

	/** x, y, z position, normal */
	VF_V3_N3 = VF_V3 | VF_N3,

	/** x, y, z position, normal, RGBA color */
	VF_V3_N3_C4 = VF_V3 | VF_N3 | VF_C4,

	/** x, y, z position, normal, uv */
	VF_V3_N3_T2 = VF_V3 | VF_N3 | VF_T2,

	/** x, y, z position, normal, uv, RGBA color */
	VF_V3_N3_T2_C4 = VF_V3 | VF_N3 | VF_T2 | VF_C4
};

class VertexFormat {
public:
	VertexFormat();
	~VertexFormat();

	/**
	 * Creates a new VertexFormat based on a template of the vertex attributes as specified in the given
	 * VertexFormatTemplates parameter.
	 *
	 * @param vfTemplate specifies the type and number of vertex attributes
	 * @return pointer to a new VertexFormat instance
	 */
	static VertexFormat* create(enum VertexFormatTemplates vfTemplate);

	VertexFormat& operator=(const VertexFormat& rhs);
	VertexAttribute* addAttribute(VertexAttribute* ve);
	VertexAttribute* addAttribute(VertexAttributeSemantic semantic, VertexAttributeFormat format, uint32_t offset,
			void* data);
	VertexAttribute* addAttribute(VertexAttributeSemantic semantic, VertexAttributeFormat format);
	VertexAttribute* getAttributeByIndex(uint8_t numElement) const;
	VertexAttribute* getAttributeBySemantic(VertexAttributeSemantic semantic) const;
	uint8_t getNumElements() const;

	void printData();
private:
	uint8_t m_numAttributes;
	VertexAttributePtr m_attributes[MAX_VERTEX_ELEMENTS];
};
typedef shared_ptr<VertexFormat> VertexFormatPtr;

#endif /* VERTEXFORMAT_H_ */
