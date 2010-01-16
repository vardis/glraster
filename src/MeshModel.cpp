/*
 * MeshModel.cpp
 *
 *  Created on: Nov 29, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "MeshModel.h"

MeshModel::MeshModel(Mesh* mesh) :
	Renderable(), m_mesh(mesh) {
	m_material = m_mesh->getMaterial();
}

MeshModel::~MeshModel() {
}

void MeshModel::renderGeometry() {

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_RESCALE_NORMAL);

	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	VertexFormat* vf = m_mesh->getVertexFormat();
	for (uint8_t i = 0; i < vf->getNumElements(); i++) {
		VertexElement* ve = vf->getElementByIndex(i);
		ve->m_vbo->uploadData();
	}

	MaterialPtr mat = m_mesh->getMaterial();
	if (mat) {
		for (uint8_t i = 0; i < MAX_TEXTURES_STACK; i++) {
			TexturePtr tex = mat->m_textures->textures[i];
			if (tex && tex->m_texID && glIsTexture(tex->m_texID)) {
				glEnable(tex->m_textureTarget);
				glActiveTexture(GL_TEXTURE0 + i);
				glClientActiveTexture(GL_TEXTURE0 + i);

//				VertexElement* ve = vf->getElementBySemantic((VertexElementSemantic) (Vertex_TexCoord0
//						+ mat->m_textures->texInputs[i].uvSet));
//				ve->m_vbo->uploadData();
			}
		}
	}

	m_mesh->getIbo()->drawPrimitives();
	glPopClientAttrib();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshModel::_updateBounds() {
	//TODO: Remove this and calculate bounds upon the mesh creation
	VertexElement* ve = m_mesh->getVertexFormat()->getElementByIndex(Vertex_Pos);
	size_t formatSize = VertexElement::getFormatSize(ve->m_format);
	float x, y, z;
	float* fData;
	short* sData;
	void* data = ve->m_vbo->mapData();
	for (uint32_t i = 0; i < m_mesh->getNumVertices(); i++) {
		fData = 0;
		sData = 0;
		switch (ve->m_format) {
		case VertexFormat_SHORT2:
			sData = reinterpret_cast<short*> (data);
			x = sData[i * formatSize];
			y = sData[i * formatSize + 1];
			z = 0.0f;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (data);
			x = sData[i * formatSize];
			y = sData[i * formatSize + 1];
			z = sData[i * formatSize + 2];
			break;
		case VertexFormat_FLOAT2:
			fData = reinterpret_cast<float*> (data);
			x = fData[i * formatSize];
			y = fData[i * formatSize + 1];
			z = 0.0f;
			break;
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (data);
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
	ve->m_vbo->unmapData();
}
