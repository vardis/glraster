/*
 * VertexFormat.cpp
 *
 *  Created on: Nov 27, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "VertexFormat.h"

VertexFormat::VertexFormat() :
	m_numAttributes(0), m_attributes() {

}

VertexFormat::~VertexFormat() {
}

VertexFormat* VertexFormat::create(enum VertexFormatTemplates vfTemplate) {
	VertexFormat* vf = 0;
	switch (vfTemplate) {
	case VF_V2:
		vf = new VertexFormat();
		vf->addAttribute(Vertex_Pos, VertexFormat_FLOAT2);
		return vf;
		break;
	case VF_V3:
		vf = new VertexFormat();
		vf->addAttribute(Vertex_Pos, VertexFormat_FLOAT3);
		return vf;
		break;
	case VF_V3_C4:
		vf = new VertexFormat();
		vf->addAttribute(Vertex_Pos, VertexFormat_FLOAT3);
		vf->addAttribute(Vertex_Color, VertexFormat_FLOAT4);
		return vf;
		break;
	case VF_V3_N3_C4:
		vf = new VertexFormat();
		vf->addAttribute(Vertex_Pos, VertexFormat_FLOAT3);
		vf->addAttribute(Vertex_Normal, VertexFormat_FLOAT3);
		vf->addAttribute(Vertex_Color, VertexFormat_FLOAT4);
		return vf;
		break;
	case VF_V3_N3:
		vf = new VertexFormat();
		vf->addAttribute(Vertex_Pos, VertexFormat_FLOAT3);
		vf->addAttribute(Vertex_Normal, VertexFormat_FLOAT3);
		return vf;
		break;
	case VF_V3_T2:
		vf = new VertexFormat();
		vf->addAttribute(Vertex_Pos, VertexFormat_FLOAT3);
		vf->addAttribute(Vertex_TexCoord0, VertexFormat_FLOAT2);
		return vf;
		break;
	case VF_V3_N3_T2:
		vf = new VertexFormat();
		vf->addAttribute(Vertex_Pos, VertexFormat_FLOAT3);
		vf->addAttribute(Vertex_Normal, VertexFormat_FLOAT3);
		vf->addAttribute(Vertex_TexCoord0, VertexFormat_FLOAT2);
		return vf;
		break;
	case VF_V3_N3_T2_C4:
		vf = new VertexFormat();
		vf->addAttribute(Vertex_Pos, VertexFormat_FLOAT3);
		vf->addAttribute(Vertex_Normal, VertexFormat_FLOAT3);
		vf->addAttribute(Vertex_TexCoord0, VertexFormat_FLOAT2);
		vf->addAttribute(Vertex_Color, VertexFormat_FLOAT4);
		return vf;
		break;
	default:
		SAFE_THROW(GLException(E_BADARG, "Unknown vertex format template"))
		;
	}
	return 0;
}

VertexFormat& VertexFormat::operator=(const VertexFormat& rhs) {
	m_numAttributes = rhs.m_numAttributes;
	for (uint8_t i = 0; i < m_numAttributes; i++) {
		// copy pointers
		m_attributes[i] = rhs.m_attributes[i];
	}
	return *this;
}

VertexAttribute* VertexFormat::addAttribute(VertexAttribute* ve) {
	m_attributes[m_numAttributes] = VertexAttributePtr(ve);
	++m_numAttributes;
	return ve;
}

VertexAttribute* VertexFormat::addAttribute(VertexAttributeSemantic semantic, VertexAttributeFormat format,
		uint32_t offset, void* data) {
	assert(m_numAttributes <= MAX_VERTEX_ELEMENTS && !this->getAttributeBySemantic(semantic));
	return this->addAttribute(new VertexAttribute(semantic, format, offset, data));
}

VertexAttribute* VertexFormat::addAttribute(VertexAttributeSemantic semantic, VertexAttributeFormat format) {
	assert(m_numAttributes <= MAX_VERTEX_ELEMENTS);
	return this->addAttribute(new VertexAttribute(semantic, format, 0, 0));
}

VertexAttribute* VertexFormat::getAttributeByIndex(uint8_t numElement) const {
	assert(numElement < MAX_VERTEX_ELEMENTS);
	return m_attributes[numElement].get();
}

VertexAttribute* VertexFormat::getAttributeBySemantic(VertexAttributeSemantic semantic) const {
	for (uint8_t i = 0; i < m_numAttributes; i++) {
		if (m_attributes[i]->m_semantic == semantic) {
			return m_attributes[i].get();
		}
	}
	return 0;
}

uint8_t VertexFormat::getNumElements() const {
	return m_numAttributes;
}

void VertexFormat::printData() {
	for (uint8_t i = 0; i < m_numAttributes; i++) {
		VertexAttribute* va = this->getAttributeByIndex(i);
		float* fp = (float*) va->m_vbo->mapData();
		std::cout << "Print attribute with semantic: " << va->m_semantic << " having " << va->m_vbo->m_numElements
				<< " elements\n";
		uint32_t count = va->getFormatElementCount(va->m_format);
		for (uint32_t e = 0, c = 0; e < va->m_vbo->m_numElements * count; e++) {
			if (!c)
				std::cout << "(";

			std::cout << *fp++ << ", ";

			if (c == count - 1) {
				std::cout << ")\n";
				c = 0;
			} else {
				++c;
			}
		}
		std::cout << ")\n";
		va->m_vbo->unmapData();
	}
}
