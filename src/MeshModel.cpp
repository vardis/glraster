/*
 * MeshModel.cpp
 *
 *  Created on: Nov 29, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "MeshModel.h"

MeshModel::MeshModel(Mesh* mesh) :
	Renderable(), m_mesh(mesh), m_vboID(0), m_iboID(0) {
	m_material = m_mesh->getMaterial();
}

MeshModel::~MeshModel() {
	if (m_vboID) {
		glDeleteBuffers(1, &m_vboID);
	}

	if (m_iboID) {
		glDeleteBuffers(1, &m_iboID);
	}
}

void MeshModel::renderGeometry() {

	if (m_mesh->isVertexDataUpdated()) {
		_updateBounds();
		_updateVertexBuffer();
		m_mesh->setVertexDataUpdated(false);
	}
	if (m_mesh->isIndexDataUpdated()) {
		_updateIndicesBuffer();
		m_mesh->setIndexDataUpdated(false);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_RESCALE_NORMAL);

	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	VertexFormat* vf = m_mesh->getVertexFormat();
	for (uint8_t i = 0; i < vf->getNumElements(); i++) {
		VertexElement* ve = vf->getElementByIndex(i);
		glBindBuffer(GL_ARRAY_BUFFER, ve->m_vboID);
		switch (ve->m_semantic) {
		case Vertex_Pos:
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(VertexElement::getFormatElementCount(ve->m_format), VertexElement::getFormatType(
					ve->m_format), 0, BUFFER_OFFSET(0));
			break;

		case Vertex_Color:
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(VertexElement::getFormatElementCount(ve->m_format), VertexElement::getFormatType(
					ve->m_format), 0, BUFFER_OFFSET(0));
			break;
		case Vertex_Normal:
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(VertexElement::getFormatType(ve->m_format), 0, BUFFER_OFFSET(0));
			break;
			//		case Vertex_TexCoord0:
			//		case Vertex_TexCoord1:
			//		case Vertex_TexCoord2:
			//		case Vertex_TexCoord3:
			//		case Vertex_TexCoord4:
			//		case Vertex_TexCoord5:
			//		case Vertex_TexCoord6:
			//		case Vertex_TexCoord7:
			//			glActiveTexture(GL_TEXTURE0 + (ve->m_semantic - Vertex_TexCoord0));
			// GL_TEXTURE_1D, GL_TEXTURE_2D or GL_TEXTURE_3D will have been already enabled when
			// the material is applied in the RenderablesRasterizer
			//			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			//			glClientActiveTexture(GL_TEXTURE0 + (ve->m_semantic - Vertex_TexCoord0));
			//			glTexCoordPointer(VertexElement::getFormatElementCount(ve->m_format), VertexElement::getFormatType(
			//					ve->m_format), 0, BUFFER_OFFSET(0));
		default:
			break;
		}
	}

	MaterialPtr mat = m_mesh->getMaterial();
	if (mat) {
		for (uint8_t i = 0; i < MAX_TEXTURES_STACK; i++) {
			TexturePtr tex = mat->m_textures->textures[i];
			if (tex && tex->m_texID && glIsTexture(tex->m_texID)) {
				glEnable(tex->m_textureTarget);
				glActiveTexture(GL_TEXTURE0 + i);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				VertexElement* ve = vf->getElementBySemantic((VertexElementSemantic) (Vertex_TexCoord0
						+ mat->m_textures->texInputs[i].uvSet));
				glBindBuffer(GL_ARRAY_BUFFER, ve->m_vboID);
				glClientActiveTexture(GL_TEXTURE0 + i);
				glTexCoordPointer(VertexElement::getFormatElementCount(ve->m_format), VertexElement::getFormatType(
						ve->m_format), 0, BUFFER_OFFSET(0));
			}
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glDrawRangeElements(GL_TRIANGLES, 0, m_mesh->getNumIndices() - 1, m_mesh->getNumIndices(), GL_UNSIGNED_INT, 0);
	glPopClientAttrib();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshModel::_updateVertexBuffer() {
	VertexFormat* vf = m_mesh->getVertexFormat();
	for (uint8_t i = 0; i < vf->getNumElements(); i++) {
		VertexElement* ve = vf->getElementByIndex(i);
		if (!glIsBuffer(ve->m_vboID)) {
			glGenBuffers(1, &ve->m_vboID);
		}
		glBindBuffer(GL_ARRAY_BUFFER, ve->m_vboID);
		glBufferData(GL_ARRAY_BUFFER, m_mesh->getNumVertices() * ve->m_size, 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_mesh->getNumVertices() * ve->m_size, ve->m_data);
	}
}

void MeshModel::_updateIndicesBuffer() {
	if (!glIsBuffer(m_iboID)) {
		glGenBuffers(1, &m_iboID);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_mesh->getNumIndices() * sizeof(uint32_t), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_mesh->getNumIndices() * sizeof(uint32_t), m_mesh->getIndices());
}

void MeshModel::_updateBounds() {
	VertexElement* ve = m_mesh->getVertexFormat()->getElementByIndex(Vertex_Pos);
	size_t formatSize = VertexElement::getFormatSize(ve->m_format);
	float x, y, z;
	float* fData;
	short* sData;
	for (uint32_t i = 0; i < m_mesh->getNumVertices(); i++) {
		fData = 0;
		sData = 0;
		switch (ve->m_format) {
		case VertexFormat_SHORT2:
			sData = reinterpret_cast<short*> (ve->m_data);
			x = sData[i * formatSize];
			y = sData[i * formatSize + 1];
			z = 0.0f;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (ve->m_data);
			x = sData[i * formatSize];
			y = sData[i * formatSize + 1];
			z = sData[i * formatSize + 2];
			break;
		case VertexFormat_FLOAT2:
			fData = reinterpret_cast<float*> (ve->m_data);
			x = fData[i * formatSize];
			y = fData[i * formatSize + 1];
			z = 0.0f;
			break;
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (ve->m_data);
			x = fData[i * formatSize];
			y = fData[i * formatSize + 1];
			z = fData[i * formatSize + 2];
			break;
		default:
			// TODO: error handling
			x = y = z = 0.0f;
			break;
		}
		m_bounds.addPoint(x, y, z);
	}
}
