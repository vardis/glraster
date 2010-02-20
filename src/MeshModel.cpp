/*
 * MeshModel.cpp
 *
 *  Created on: Nov 29, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "MeshModel.h"

MeshModel::MeshModel(Mesh* mesh) :
	Renderable(), m_mesh(mesh), m_hasBindedOnce(false) {
	m_vf = mesh->getVertexFormat();
	m_material = m_mesh->getMaterial();
	glGenVertexArrays(1, &m_vao);
}

MeshModel::~MeshModel() {
	glDeleteVertexArrays(1, &m_vao);
}

void MeshModel::renderGeometry() {

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_RESCALE_NORMAL);

	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glBindVertexArray(m_vao);
	if (!m_hasBindedOnce) {
		VertexFormatPtr vf = m_mesh->getVertexFormat();
		vf->bindData();
		std::cout << "BINDING MESHMODEL DATA\n";
		m_hasBindedOnce = true;
	}
	//	for (uint8_t i = 0; i < vf->getNumElements(); i++) {
	//		VertexAttribute* ve = vf->getAttributeByIndex(i);
	//		ve->m_vbo->uploadData();
	//	}

	MaterialPtr mat = m_mesh->getMaterial();
	if (mat) {
		for (uint8_t i = 0; i < MAX_TEXTURES_STACK; i++) {
			TexturePtr tex = mat->m_texStack->textures[i];
			if (tex && tex->m_texID && glIsTexture(tex->m_texID)) {
				glEnable(tex->m_textureTarget);
				glActiveTexture(GL_TEXTURE0 + i);
				glClientActiveTexture(GL_TEXTURE0 + i);

				//				VertexAttribute* ve = vf->getAttributeBySemantic((VertexAttributeSemantic) (Vertex_TexCoord0
				//						+ mat->m_texStack->texInputs[i].uvSet));
				//				ve->m_vbo->uploadData();
			}
		}
	}

	m_mesh->getIbo()->drawPrimitives();
	glPopClientAttrib();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

#ifdef EXPERIMENTAL_CODE
void MeshModel::getRenderOperation(RenderOp& op) {
	op.setState(m_renderState);
	op.setType(RO_Indexed_Geometry);
	op.setPrimitiveType(m_mesh->getIbo()->m_primitiveType);
	op.setVertexFormat(m_vf);
	op.setIndexBuffer(m_mesh->getIbo());
}

void MeshModel::renderAttribGeometry() {

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	m_renderState.enableRescaleNormals();
	m_renderState.enable(RS_Rescale_Normals);

	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	VertexFormat* vf = m_mesh->getVertexFormat();
	for (uint8_t i = 0; i < vf->getNumElements(); i++) {
		VertexAttribute* ve = vf->getAttributeByIndex(i);
		ve->m_vbo->uploadData();
	}

	MaterialPtr mat = m_mesh->getMaterial();
	if (mat) {
		for (uint8_t i = 0; i < MAX_TEXTURES_STACK; i++) {
			TexturePtr tex = mat->m_texStack->textures[i];
			if (tex && tex->m_texID && glIsTexture(tex->m_texID)) {
				glEnable(tex->m_textureTarget);
				glActiveTexture(GL_TEXTURE0 + i);
				glClientActiveTexture(GL_TEXTURE0 + i);

				//				VertexAttribute* ve = vf->getAttributeBySemantic((VertexAttributeSemantic) (Vertex_TexCoord0
				//						+ mat->m_texStack->texInputs[i].uvSet));
				//				ve->m_vbo->uploadData();
			}
		}
	}

	m_mesh->getIbo()->drawPrimitives();
	glPopClientAttrib();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
#endif

void MeshModel::_updateBounds() {
	//TODO: Remove this and calculate bounds upon the mesh creation
	VertexAttribute* ve = m_mesh->getVertexFormat()->getAttributeByIndex(Vertex_Pos);
	size_t formatSize = VertexAttribute::getFormatSize(ve->m_format);
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
